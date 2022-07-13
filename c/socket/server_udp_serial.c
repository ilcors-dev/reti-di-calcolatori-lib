#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "lib.h"
#define BUF_LEN 256

int main(int argc, char **argv)
{
    int sd, port, ok;
    unsigned int serv_len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    struct hostent *clienthost;
    char buf[BUF_LEN];
    // TODO init data

    // ARGS CHECK -------------------------------------------------------------------
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int nread = 0;
    while (argv[1][nread] != '\0')
    {
        if ((argv[1][nread] < '0') || (argv[1][nread] > '9'))
        {
            printf("Please provide a valid port number\n");
            exit(1);
        }

        nread++;
    }

    port = atoi(argv[1]);
    if (port < 1024 || port > 65535)
    {
        printf("Invalid port, allowed range is 1024-65535\n");
        exit(2);
    }
    // ----------------------------------------------------------------------------

    // INIT SERVER ADDRESS
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    serv_len = sizeof(struct sockaddr_in);

    // SOCKET CREATION & SETTINGS
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("Error on opening socket");
        exit(1);
    }
    printf("Server: creata la socket, sd=%d\n", sd);

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("set opzioni socket");
        exit(2);
    }
    printf("Server: set opzioni socket ok\n");

    if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Bind socket");
        exit(3);
    }
    printf("Server: bind socket ok\n");

    /* ciclo di ricezione richieste */
    for (;;)
    {
        // DO YOUR STUFF IN HERE ---------------------------------------------
        // REMEMBER, when sending integers, you must convert them to network byte order using
        // htonl() and ntohl() before sending them.
        int whatever = 0;
        printf("UDP request received\n");

        len = sizeof(struct sockaddr_in);
        if (recvfrom(udp_sd, &whatever, sizeof(whatever), 0, (struct sockaddr *)&cliaddr, &len) < 0)
        {
            perror("Error in recvfrom");
            continue;
        }

        int ris = ntohl(whatever);
        ris++;
        ris = htonl(ris);
        if (sendto(udp_sd, &ris, sizeof(ris), 0, (struct sockaddr *)&cliaddr, len) < 0)
        {
            perror("Error in sendto");
            continue;
        }
        // -------------------------------------------------------------------
    }
}
