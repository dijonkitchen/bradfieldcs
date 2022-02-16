#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>

int main(void) {
    // create/open a socket
    int socket_descriptor = -1;
    if ((socket_descriptor = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("Error creating socket; errno = %d\n", errno);

        return errno;
    }
    printf("Socket descriptor: %d\n", socket_descriptor);

    // bind to a port
    struct sockaddr_in socket_address_internet;

    memset(&socket_address_internet, 0, sizeof(socket_address_internet));
    socket_address_internet.sin_family = AF_INET;
    socket_address_internet.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_descriptor, (struct sockaddr *) &socket_address_internet, sizeof(socket_address_internet)) == -1) {
        printf("Error binding socket; errno: %d\n", errno);

        return errno;
    }
    printf("Socket data port: %hu\n", socket_address_internet.sin_port);

    // encode query message
    // header
    int id = 0b0;
    int qr = 0b1;
    int opcode = 0b0;
    int aa = 0b0;
    int tc = 0b0;
    int rd = 0b0;
    int ra = 0b0;
    int z = 0b0;
    int rcode = 0b0;
    int qdcount = 0b1;
    int ancount = 0b0;
    int nscount = 0b0;
    int arcount = 0b0;


    // send query message
    // receive response
    // decode response

    // clean up bind?
//    if (unlink(socket_address_internet.sa_data) == -1) {
//        printf("Error unlinking socket; errno: %d\n", errno);
//
//        return errno;
//    }

    return 0;
}