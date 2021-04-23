/* Copyright Chelcea Claudiu-Marian */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_balancer.h"
#include "utils.h"

int main(int argc, char* argv[])
{
	FILE *input = NULL;

	// Get file
	if (argc != 2) {
		printf("Usage:%s input_file \n", argv[0]);
		return -1;
	}

	 input = fopen(argv[1], "rt");
	//input = fopen("in","rt");
	DIE(input == NULL, "missing input file");

	// Execute command
	apply_requests(input);

	fclose(input);

	return 0;
}