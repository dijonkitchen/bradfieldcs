#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
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

  if (execvpe(config->argv[0], config->argv, NULL)) {
    fprintf(stderr, "execvpe failed %m.\n");

    return -1;
  }

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
