#ifndef __GALAXY_H_
#define __GALAXY_H_
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#define DIE(suppose, error_message)									    \
if (suppose) {														    \
    fprintf(stderr, "At line %d in file %s!\n", __LINE__, __FILE__);    \
    fprintf(stderr, "Errno value: %d with message: ", errno);		    \
	perror(error_message);											    \
	printf("\n");													    \
    exit(errno);												    	\
}

#include <stdio.h>
#include <stdlib.h>
#include "Shields.h"
#include <string.h>
#define GARBAGE - 232343

// Create planet structure
typedef struct planet
{
	char * planetName;
	struct planet* prev;
	struct planet* next;
	unsigned int shields_number;
	unsigned int distroyed_planets;
    shield_List* shieldHead;
} planet_t;


// Create the galaxy structure
typedef struct galaxy
{
	unsigned int size;
	struct planet* head;
}  galaxy_t;

// Initialise galaxy
galaxy_t* init_galaxy();

#endif