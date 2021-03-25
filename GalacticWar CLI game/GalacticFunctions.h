#ifndef __GALACTIC_F__
#define __GALACTIC_F__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "Galaxy.h"

#define MAX_COMMAND 100
#define COMMANDS 8
#define NAME_SIZE 64
#define DIE(suppose, error_message)\
if (suppose) {                                                                  \
    fprintf(stderr, "At line %d in file %s!\n", __LINE__, __FILE__);            \
    fprintf(stderr, "Errno value: %d with message: ", errno);                   \
    perror(error_message);                                                      \
    printf("\n");                                                               \
    exit(errno);                                                                \
}

// Execute the received command
void execute_command(const int command_index, char * command_line, galaxy_t ** galaxy);

// Add planet to the galaxy
void ADD(char * planet_name,
    const int planet_index,
        const int shields_number, galaxy_t ** galaxy);

// Throw planet into a blackhole
void BLH(const int planet_index, galaxy_t ** galaxy);

// Upgrade a planet
void UPG(unsigned int planet_index,
        unsigned int shield_index, unsigned
    const int upgrade_value, galaxy_t ** galaxy);

// Add a new shield to the planet
void EXP(const int planet_index,
    const int shield_value, galaxy_t ** galaxy);

// Remove a certain shield from a planet
void RMV(const int planet_index,
    const int shield_index, galaxy_t ** galaxy);

// Collide two planets
void COL(const int index_planet_1,
    const int index_planet_2, galaxy_t ** galaxy);

// Rotate a planet (and it's shields)
void ROT(const int planet_index,
    const char direction, int units, galaxy_t ** galaxy);

// Display planet info
void SHW(int planet_index, galaxy_t ** galaxy);

// Return the index of the command from the command list or -1
int return_index(const char * command,
    const char * command_list[]);

// Print int list
void dll_print_int_l(galaxy_t * list);

#endif
