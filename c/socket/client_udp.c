#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../multilib.h"

typedef struct
{
	// ...
} ReqUDP;

int main(int argc, char **argv)
{
	struct hostent *host;
	struct sockaddr_in clientaddr, servaddr;
	int port, socked_d, len = 0;
	char userInput[STR_LEN];
	// ReqUDP req;

	// ARGS CHECK -------------------------------------------------------------------
	if (argc != 3)
	{
		printf("Error:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}

	int nread = 0;
	while (argv[2][nread] != '\0')
	{
		if ((argv[2][nread] < '0') || (argv[2][nread] > '9'))
		{
			printf("Please provide a valid port number\n");
			exit(1);
		}

		nread++;
	}

	port = atoi(argv[2]);
	if (port < 1024 || port > 65535)
	{
		printf("Error:%s serverPort must be in the range 1024-65535\n", argv[2]);
		exit(2);
	}
	// ----------------------------------------------------------------------------

	// client address initialization
	memset((char *)&clientaddr, 0, sizeof(struct sockaddr_in));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr == INADDR_ANY;
	clientaddr.sin_port = 0;

	// server address initialization
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);

	// host check
	if (host == NULL)
	{
		printf("%s not found in /etc/hosts\n", argv[1]);
		exit(2);
	}
	else
	{
		servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
		servaddr.sin_port = htons(port);
	}

	// socket creation
	socked_d = socket(AF_INET, SOCK_DGRAM, 0);
	if (socked_d < 0)
	{
		perror("Cloud not open udp socket");
		exit(1);
	}
	printf("Successfully created udp socket, socked_d=%d\n", socked_d);

	// socket binding
	if (bind(socked_d, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
	{
		perror("bind socket ");
		exit(1);
	}
	printf("Successfully bind socket at port %i\n", clientaddr.sin_port);

	// user interaction

	printf("Waiting user input...\n");
	while (gets(userInput))
	{
		// REMEMBER, when sending integers, you must convert them to network byte order using
		// htonl() and ntohl() before sending them.
		int whatever;
		len = sizeof(servaddr);
		if (sendto(socked_d, userInput, sizeof(userInput), 0, (struct sockaddr *)&servaddr, len) < 0)
		{
			perror("Could not send message with sendto");
			// if error happens, continue cycle anyway
			printf("Waiting user input...\n");
			continue;
		}

		if (recvfrom(socked_d, &whatever, sizeof(whatever), 0, (struct sockaddr *)&servaddr, &len) < 0)
		{
			perror("Could not receive message with recvfrom");
			// if error happens, continue cycle anyway
			printf("Waiting user input...\n");
			continue;
		}
		whatever = ntohl(whatever);

		printf("Result: %i\n", whatever);
		printf("Waiting user input...\n");
	}

	close(socked_d);
}