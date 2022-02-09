#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <linux/capability.h>

#define STACK_SIZE 65536

struct child_config {
  int argc;
  char **argv;
};

int drop_capabilities() {
  int capabilities_to_drop[] = {
    CAP_DAC_OVERRIDE
  };

  prctl(PR_CAPBSET_DROP, CAP_DAC_OVERRIDE, 0, 0, 0);

  return 0;
}

int pivot_root(const char *new_root, const char *put_old)
{
	return syscall(SYS_pivot_root, new_root, put_old);
}

int mounts()
{
	fprintf(stderr, "=> remounting everything with MS_PRIVATE...");
	if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL)) {
		fprintf(stderr, "failed! %m\n");
		return -1;
	}
	fprintf(stderr, "remounted.\n");

	fprintf(stderr, "=> making a temp directory and a bind mount there...");
	char mount_dir[] = "/tmp/tmp.XXXXXX";
	if (!mkdtemp(mount_dir)) {
		fprintf(stderr, "failed making a directory!\n");
		return -1;
	}

	if (mount("./new_root", mount_dir, "proc", MS_BIND | MS_PRIVATE, NULL)) {
		fprintf(stderr, "bind mount failed!\n");
		return -1;
	}
  	mount("proc", "/proc", "proc", 0, 0);

	char inner_mount_dir[] = "/tmp/tmp.XXXXXX/oldroot.XXXXXX";
	memcpy(inner_mount_dir, mount_dir, sizeof(mount_dir) - 1);
	if (!mkdtemp(inner_mount_dir)) {
		fprintf(stderr, "failed making the inner directory!\n");
		return -1;
	}
	fprintf(stderr, "done.\n");

	fprintf(stderr, "=> pivoting root...");
	if (pivot_root(mount_dir, inner_mount_dir)) {
		fprintf(stderr, "failed!\n");
		return -1;
	}
	fprintf(stderr, "done.\n");

	char *old_root_dir = basename(inner_mount_dir);
	char old_root[sizeof(inner_mount_dir) + 1] = { "/" };
	strcpy(&old_root[1], old_root_dir);

	fprintf(stderr, "=> unmounting %s...", old_root);
	if (chdir("/")) {
		fprintf(stderr, "chdir failed! %m\n");
		return -1;
	}
	if (umount2(old_root, MNT_DETACH)) {
		fprintf(stderr, "umount failed! %m\n");
		return -1;
	}
	if (rmdir(old_root)) {
		fprintf(stderr, "rmdir failed! %m\n");
		return -1;
	}
	fprintf(stderr, "done.\n");
	return 0;
}


/* Entry point for child after `clone` */
int child(void *arg) {
  struct child_config *config = arg;

  const pid_t pid = getpid();
  if (pid == 1) {
    printf("PID namespaces are working\n");
  } else {
    printf("PID namespaces ARE NOT working. Child pid: %d\n", pid);
  }
  
  drop_capabilities();

  mounts();
  // chroot("./new_root");
  // chdir("/");
  // mount("proc", "/proc", "proc", 0, 0);

  if (execvpe(config->argv[0], config->argv, NULL)) {
    fprintf(stderr, "execvpe failed %m.\n");

    return -1;
  }

  umount("/proc");

  return 0;
}


int main(int argc, char**argv) {
  struct child_config config = {0};
  int flags = 0 | CLONE_NEWNET | CLONE_NEWPID | CLONE_NEWIPC 
	  | CLONE_NEWNS | CLONE_NEWUSER | CLONE_NEWUTS | CLONE_NEWCGROUP;
  pid_t child_pid = 0;

  // Prepare child configuration
  config.argc = argc - 1;
  config.argv = &argv[1];

  // Allocate stack for child
  char *stack = 0;
  if (!(stack = malloc(STACK_SIZE))) {
    fprintf(stderr, "Malloc failed");

    exit(1);
  }

  // Clone parent, enter child code
  if ((child_pid = clone(child, stack + STACK_SIZE, flags | SIGCHLD, &config)) == -1) {
    fprintf(stderr, "Clone failed");

    exit(2);
  }

  wait(NULL);
  
  return 0;
}
