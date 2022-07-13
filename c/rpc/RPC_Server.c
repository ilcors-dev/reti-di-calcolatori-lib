#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "RPC_xFile.h"

static TestStruct data[TESTS_LEN];
static int booted = 0;

/**
 * @brief Prints the print TestStruct array passed as argument
 *
 * @param print the array to print
 * @param len len of the array
 */
void printData(TestStruct *print, int len)
{
	for (int i = 0; i < len; i++)
		printf("%s, %s, %s, %s\n", print[i].id, print[i].bookerId, print[i].brand, print[i].imagesFolder);
}

int boot()
{
	printf("Booting...\n");

	for (int i = 0; i < TESTS_LEN; i++)
	{
		data[i].id = malloc(8 * sizeof(char));
		data[i].bookerId = malloc(5 * sizeof(char));
		data[i].brand = malloc(STR_LEN * sizeof(char));
		data[i].imagesFolder = malloc(strlen("vehicles/AA777BB") * sizeof(char));
	}

	strcpy(data[0].id, "AA777BB");
	strcpy(data[0].bookerId, "L");
	strcpy(data[0].brand, "brand1");
	strcpy(data[0].imagesFolder, "vehicles/AA777BB");

	strcpy(data[1].id, "BB777CC");
	strcpy(data[1].bookerId, "22222");
	strcpy(data[1].brand, "brand2");
	strcpy(data[1].imagesFolder, "vehicles/BB777CC");

	strcpy(data[2].id, "FF777EE");
	strcpy(data[2].bookerId, "13223");
	strcpy(data[2].brand, "brand1");
	strcpy(data[2].imagesFolder, "vehicles/FF777EE");

	strcpy(data[3].id, "EE888GG");
	strcpy(data[3].bookerId, "L");
	strcpy(data[3].brand, "L");
	strcpy(data[3].imagesFolder, "L");

	strcpy(data[4].id, "OO321HH");
	strcpy(data[4].bookerId, "L");
	strcpy(data[4].brand, "L");
	strcpy(data[4].imagesFolder, "L");

	printData(data, TESTS_LEN);

	booted = 1;
}

int *with_input_struct_1_svc(Input *input, struct svc_req *cl)
{
	static int ret;
	ret = -1;

	if (!booted)
		boot();

	// do whatever you need to do here
	if (input->intType == 0 && strcmp(input->strType, "ueue"))
	{
		return &ret;
	}

	ret = 0;

	return &ret;
}

Output *with_output_struct_1_svc(char *input, struct svc_req *cl)
{
	static Output ret;

	if (!booted)
		boot();

	// init Output struct
	for (int i = 0; i < OUTPUT_LEN; i++)
	{
		ret.tests[i].id = malloc(strlen("$"));
		ret.tests[i].bookerId = malloc(strlen("$"));
		ret.tests[i].brand = malloc(strlen("$"));
		ret.tests[i].imagesFolder = malloc(strlen("$"));
	}

	// do whatever you need to do here

	return (&ret);
}
