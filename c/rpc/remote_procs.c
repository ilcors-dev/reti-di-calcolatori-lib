#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "sudo_prod.h"

TestStruct data[TESTS_LEN];

Risultato *test_1_svc(int *req, struct svc_req *cl)
{
	static Risultato resp;
	printf("inside %s\nparam: %d\n", __func__, *req);
	resp.res = *req;
	memcpy(resp.tests, data, TESTS_LEN * sizeof(TestStruct));
	printf("exi %s\nparam: %d\n", __func__, *req);
	return &resp;
}
