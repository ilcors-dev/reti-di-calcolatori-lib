/* echo_client.c
 *	+include echo.h
 */

#include <stdio.h>
#include <rpc/rpc.h>
#include "op.h"
#define DIM 100

int main(int argc, char *argv[]){

	CLIENT *cl;
	Operandi op;
	char *server;
	int *ris;

	if (argc != 5)  {
		fprintf(stderr, "uso:%s host tipo op1 op2\n", argv[0]);
		exit(1);
	}
	if (argv[2][0] != 'm' && argv[2][0] != 's') { 
		fprintf(stderr, "uso: %s host somma/moltiplicazione op1 op2\n",argv[0]);
		fprintf(stderr, "tipo deve iniziare per 's' o 'm'\n");
		exit(1);
	}

	server = argv[1];
	op.op1 = atoi(argv[3]); 
	op.op2 = atoi(argv[4]);

	server = argv[1];

	cl = clnt_create(server, OPERAZIONIPROG, OPERAZIONIVERS, "udp");
	if (cl == NULL) {
		clnt_pcreateerror(server);
		exit(1);
	}

	if(argv[2][0] == 's'){
		ris = somma_1(&op, cl);
	}
	if(argv[2][0] == 'm'){
		ris = moltiplicazione_1(&op, cl);
	}


	if (ris == NULL){ 
		clnt_perror(cl, server); exit(1); 
	}

	printf("Risultato da %s: %i\n",server, *ris);

	clnt_destroy(cl);
	printf("Termino...\n");
	exit(0);
}
