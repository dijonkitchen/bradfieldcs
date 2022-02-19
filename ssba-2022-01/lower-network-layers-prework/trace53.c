#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>

int main(void) {
    // create/open a socket
    int socket_descriptor = -1;
    if ((socket_descriptor = socket(PF_INET, SOCK_DGRAM, IPPROTO_ICMP)) == -1) {
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
    // echo message ICMP header; https://www.rfc-editor.org/rfc/rfc792.html
    int type = 8;
    int code = 0;
    int checksum = 0;
    int identifier = 0;
    int sequence_number = 0;
    int data = 0;

    // IP header
    // source address
    // destination address


    // send; recvfrom
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