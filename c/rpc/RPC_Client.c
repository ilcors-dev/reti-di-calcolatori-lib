#include <stdio.h>
#include <errno.h>

#include "RPC_xFile.h"

int main(int argc, char **argv)
{
	CLIENT *clnt;
	Input *input_struct;
	char *server;
	char userInput[STR_LEN];
	char *service_desc = "a: azione, b: qualcos'altro\n";
	int ok = 0;

	// check arguments
	if (argc != 2)
	{
		printf("Usage: %s addr", argv[0]);
	}

	// init structures
	input_struct = malloc(sizeof(Input));
	input_struct->strType = (char *)malloc(STR_LEN);

	// rpc init
	server = argv[1];
	clnt = clnt_create(server, SERVPROG, SERVVERS, "udp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(server);
		exit(1);
	}

	// main loop for user interaction
	puts(service_desc);

	while (gets(userInput))
	{
		// first service
		if (!strcmp(userInput, "a"))
		{
			Output *resp;
			unsigned int i, j;

			gets(userInput);
			resp = with_output_struct_1(userInput, clnt);

			// check the server response, if null something went wrong with the rpc call
			if (resp == NULL)
			{
				clnt_perror(clnt, server);
				exit(1);
			}

			printf("response received from server..\n");

			for (i = 0; i < TESTS_LEN; i++)
			{
				printf("%s", resp->tests[i].id);
				printf("%s", resp->tests[i].bookerId);
				printf("%s", resp->tests[i].brand);
				printf("%s", resp->tests[i].imagesFolder);
				printf("\n");
			}
		}
		// second service
		else if (!strcmp(userInput, "e"))
		{
			printf("insert str first param:\n");
			gets(input_struct->strType);

			// if the userInput should be numeric, check it before assigning it to the userInput struct
			do
			{
				ok = 1;
				printf("insert int second param:\n");
				gets(userInput);

				int nread = 0;
				while (userInput[nread] != '\0')
				{
					if ((userInput[nread] < '0') || (userInput[nread] > '9'))
					{
						ok = 0;
					}

					nread++;
				}

				if (ok == 1)
				{
					input_struct->intType = atoi(userInput);
				}
			} while (!ok);

			int *ris;
			ris = with_input_struct_1(input_struct, clnt);

			// check the server response, if null something went wrong with the rpc call
			if (ris == NULL)
			{
				clnt_perror(clnt, server);
				exit(1);
			}

			printf("response received from server..\n");

			if (*ris == -1)
			{
				printf("Could not complete the request\n");
			}
			else
			{
				printf("yay! %d\n", *ris);
			}
		}

		puts(service_desc);
	}
	clnt_destroy(clnt);
}
