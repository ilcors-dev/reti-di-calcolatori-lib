#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "../multilib.h"

int main(int argc, char *argv[])
{
	// socket handling variables
	int port, nread, socket_d, nwrite;
	char userInput[STR_LEN];
	struct hostent *host;
	struct sockaddr_in servaddr;

	// your data here

	// ARGS CHECK -------------------------------------------------------------------
	if (argc != 3)
	{
		printf("Error:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}

	if (is_numeric_string(argv[2]) == 0)
	{
		printf("Error:%s serverPort must be a number\n", argv[2]);
		exit(1);
	}

	port = atoi(argv[2]);
	if (is_port_valid(port) == 0)
	{
		printf("Error:%s serverPort must be in the range 1024-65535\n", argv[2]);
		exit(2);
	}

	// server tcp address initialization
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);
	if (host == NULL)
	{
		printf("%s not found in /etc/hosts\n", argv[1]);
		exit(1);
	}

	servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
	servaddr.sin_port = htons(port);

	// !NOTE THE CONNECTION TO THE SERVER IS ESTABLISHED OUTSIDE OF THE LOOP, THIS MAY VARY DEPENDING ON THE
	// !SERVER'S REQUIREMENTS IMPLEMENTATION

	// creating socket
	socket_d = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_d < 0)
	{
		perror("Could not create socket");
		exit(3);
	}
	printf("Successfully created socket, socket_d=%d\n", socket_d);

	// bind connect
	if (connect(socket_d, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
	{
		perror("Could not connect to server");
		exit(3);
	}
	printf("Connection established successfully\n");

	// !NOTE END

	// user interaction

	printf("Waiting user input...\n");
	while (gets(userInput))
	{
		// handle server interaction
		// REMEMBER, when sending integers, you must convert them to network byte order using
		// htonl() and ntohl() before sending them.
		int whatever;
		printf("Sending to server: %s\n", userInput);

		nwrite = write(socket_d, userInput, strlen(userInput));

		while ((nread = read(socket_d, &whatever, sizeof(int))) > 0)
		{
			whatever = ntohl(whatever);
			printf("%d\n", whatever);
		}

		printf("Waiting user input...\n");
		// -------------------------
	}
	close(socket_d);
	printf("Client, shutting down...");
	exit(0);
}