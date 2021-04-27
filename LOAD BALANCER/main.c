/* Copyright Chelcea Claudiu-Marian */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_balancer.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	FILE *input = NULL;

	// Check if we have two arguments
	if (argc != 2) {
		printf("Usage:%s input_file \n", argv[0]);
		return -1;
	}

	// Open the file
	input = fopen(argv[1], "rt");
	DIE(input == NULL, "missing input file");

	// Execute the commands
	apply_requests(input);

	// Close the file
	fclose(input);

	return 0;
}
