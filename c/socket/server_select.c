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

#include "../multilib.h"

#define DIM_BUFF 256
#define LENGTH_FILE_NAME 20
#define max(a, b) ((a) > (b) ? (a) : (b))

void sig_chld_handler(int signo)
{
	int stato;
	wait(&stato);
}

int main(int argc, char **argv)
{
	// socket handling variables
	const int on = 1;
	int socket_conn_fd, tcp_fd, udp_fd, nready, maxfdp1;
	int len, port;
	fd_set rset;
	struct sockaddr_in cliaddr, servaddr;

	// your data here

	// ARGS CHECK -------------------------------------------------------------------
	if (argc != 2)
	{
		printf("Error: %s port\n", argv[0]);
		exit(1);
	}

	if (is_numeric_string(argv[1]) == 0)
	{
		printf("The server port must be a number\n");
		exit(1);
	}

	port = atoi(argv[1]);
	if (is_port_valid(port) == 0)
	{
		printf("Invalid port, allowed range is 1024-65535\n");
		exit(2);
	}
	// ----------------------------------------------------------------------------

	// INIT SERVER AND BIND
	memset((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);
	printf("Server started\n");

	// TCP SOCKET CREATION
	tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_fd < 0)
	{
		perror("Error on opening TCP socket");
		exit(1);
	}
	printf("TCP listen open successful, fd=%d\n", tcp_fd);

	if (setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		perror("set opzioni socket TCP");
		exit(2);
	}
	printf("Set opzioni socket TCP ok\n");

	if (bind(tcp_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("Bind socket TCP");
		exit(3);
	}
	printf("Bind socket TCP ok\n");

	if (listen(tcp_fd, 5) < 0)
	{
		perror("Could not listen on socket");
		exit(4);
	}
	printf("Listen ok\n");

	/* CREAZIONE SOCKET UDP ------------------------------------------------ */
	udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_fd < 0)
	{
		perror("Error while opening udp socket");
		exit(5);
	}
	printf("UDP socket created successfully, fd=%d\n", udp_fd);

	if (setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		perror("Could not set udp settings");
		exit(6);
	}
	printf("UDP settings configuration ok\n");

	if (bind(udp_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("Bind socket UDP");
		exit(7);
	}
	printf("Bind socket UDP ok\n");

	// listen to children termination and delegate the termination to the handle fn
	signal(SIGCHLD, sig_chld_handler);

	// setting file descriptions masks
	FD_ZERO(&rset);
	maxfdp1 = max(tcp_fd, udp_fd) + 1;

	// event handling loop with select primitive
	for (;;)
	{
		FD_SET(tcp_fd, &rset);
		FD_SET(udp_fd, &rset);

		if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0)
		{
			if (errno == EINTR)
				continue;
			else
			{
				perror("Error on select primitive");
				exit(8);
			}
		}

		// handle tcp connection
		if (FD_ISSET(tcp_fd, &rset))
		{
			printf("TCP request received\n");
			len = sizeof(struct sockaddr_in);
			if ((socket_conn_fd = accept(tcp_fd, (struct sockaddr *)&cliaddr, &len)) < 0)
			{
				if (errno == EINTR)
					continue;
				else
				{
					perror("Error on tcp request accept");
					exit(9);
				}
			}

			// spawn a child to handle the request
			if (fork() == 0)
			{
				int whatever, nread;
				close(tcp_fd);
				printf("Running into spawned child, pid=%i\n", getpid());

				// DO YOUR STUFF IN HERE ---------------------------------------------
				while ((nread = read(socket_conn_fd, &whatever, sizeof(int))) > 0)
				{
					whatever++;
					write(socket_conn_fd, &whatever, sizeof(int));
				}
				// -------------------------------------------------------------------

				// terminate connection
				printf("Terminating child %d..\n", getpid());
				shutdown(socket_conn_fd, 0);
				shutdown(socket_conn_fd, 1);
				close(socket_conn_fd);
				exit(0);
			}
		}

		// handle udp connection
		if (FD_ISSET(udp_fd, &rset))
		{
			// DO YOUR STUFF IN HERE ---------------------------------------------
			int whatever;
			printf("UDP request received\n");

			len = sizeof(struct sockaddr_in);
			if (recvfrom(udp_fd, &whatever, sizeof(whatever), 0, (struct sockaddr *)&cliaddr, &len) < 0)
			{
				perror("Error in recvfrom");
				continue;
			}

			int ris = whatever;
			if (sendto(udp_fd, &ris, sizeof(ris), 0, (struct sockaddr *)&cliaddr, len) < 0)
			{
				perror("Error in sendto");
				continue;
			}
			// -------------------------------------------------------------------
		}
	}
}
