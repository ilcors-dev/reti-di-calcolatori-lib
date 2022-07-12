#include <stdio.h>
#include <errno.h>

#include "sudo_prod.h"

#define STR_LEN 256

int main(int argc, char **argv)
{
	CLIENT *clnt;
	char *server;
	char input[STR_LEN];
	char *service_desc = "a: azione, b: qualcos'altro\n";

	if (argc != 2)
	{
		printf("Usage: %s addr", argv[0]);
	}

	server = argv[1];
	clnt = clnt_create(server, SERVPROG, SERVVERS, "udp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(server);
		exit(1);
	}

	puts("input:\n");
	puts(service_desc);
	while (gets(input))
	{
		if (!strcmp(input, "a"))
		{
			Risultato *resp;
			unsigned int i, j;

			gets("%d", &i);
			resp = test_1(&i, clnt);

			if (resp == NULL)
			{
				clnt_perror(clnt, server);
				exit(1);
			}

			printf("res: %d\n", resp->res);

			for (i = 0; i < TESTS_LEN; i++)
			{
				printf("%s ", resp->tests[i].nome);
				for (j = 0; j < resp->tests[i].arr.arr_len; j++)
				{
					printf("%d ", resp->tests[i].arr.arr_val[j]);
				}
				putchar('\n');
			}
		}
		else if (!strcmp(input, "e"))
		{
			printf("a\n");
		}
		puts(service_desc);
	}
	clnt_destroy(clnt);
}
