#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void sig_chld_handler(int signo)
{
    int stato;
    wait(&stato);
}

int main(int argc, char **argv)
{
    int listen_sd, conn_sd, nread, yolo;
    int port, len, ok;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    struct hostent *host;
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

    // TCP SOCKET CREATION
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sd < 0)
    {
        perror("Error on opening TCP socket");
        exit(1);
    }
    printf("Server: creata la socket d'ascolto per le richieste, fd=%d\n", listen_sd);

    if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("set opzioni socket TCP");
        exit(2);
    }
    printf("Server: set opzioni socket d'ascolto ok\n");

    if (bind(listen_sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Bind socket TCP");
        exit(3);
    }
    printf("Server: bind socket d'ascolto ok\n");

    // listen on tcp queue of 5
    if (listen(listen_sd, 5) < 0)
    {
        perror("Could not listen on socket");
        exit(4);
    }
    printf("Server: listen ok\n");

    // listen to children termination and delegate the termination to the handle fn
    signal(SIGCHLD, sig_chld_handler);

    // listen for requests
    for (;;)
    {
        len = sizeof(cliaddr);
        if ((conn_sd = accept(listen_sd, (struct sockaddr *)&cliaddr, &len)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
            {
                perror("Error on tcp request accept");
                exit(9);
            }
        }

        if (fork() == 0)
        {
            int whatever, nread;
            close(listen_sd);
            printf("Running into spawned child, pid=%i\n", getpid());

            host = gethostbyaddr((char *)&cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
            if (host == NULL)
            {
                printf("client host information not found\n");
                close(conn_sd);
                exit(6);
            }
            else
            {
                printf("Request received from: %s \n", host->h_name);
            }

            // DO YOUR STUFF IN HERE ---------------------------------------------
            while ((nread = read(conn_sd, &whatever, sizeof(int))) > 0)
            {
                whatever = ntohl(whatever);
                whatever++;
                whatever = htonl(whatever);
                write(conn_sd, &whatever, sizeof(int));
            }
            // -------------------------------------------------------------------

            // terminate connection
            printf("Terminating child %d..\n", getpid());
            shutdown(conn_sd, 0);
            shutdown(conn_sd, 1);
            close(conn_sd);
            exit(0);
        }
        close(conn_sd);
    }
}