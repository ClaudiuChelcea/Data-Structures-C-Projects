#ifndef __GALAXY__H_
#define __GALAXY__H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NAME_SIZE 64
#define DIE(suppose, error_message)                                             \
if (suppose) {                                                                  \
    printf("At line %d in file %s!\n", __LINE__, __FILE__);                     \
    printf("Errno value: %d with message: ", errno);                            \
    perror(error_message);                                                      \
    printf("\n");                                                               \
    exit(errno);                                                                \
}

// Generic node
typedef struct galaxy_object {
    void * data;
    struct galaxy_object * prev;
    struct galaxy_object * next;
}
galaxy_object;

// Galaxy, the list
typedef struct galaxy_t {
    galaxy_object * head;
    unsigned int galaxy_size;
}
galaxy_t;

// Data
typedef struct only_data {
    char * name;
    unsigned int shields_number;
    unsigned int destroyed_planets;
    galaxy_t * shield;
}
only_data_t;

// Create the galaxy
galaxy_t *
    dll_create();

// Add node to nth position
void
dll_add_nth_node_shield(galaxy_t * list, unsigned int n,
    const void * data);

// Add first shield
void add_First(galaxy_t * list, void * new_data);

// Return the nth node of the galaxy
galaxy_object *
    dll_get_nth_node(galaxy_t * list, unsigned int n);

// Add a node at the nth position
void
dll_add_nth_node(galaxy_t ** list, unsigned int planet_index, int shields_number, char * planet_name, int* global_size);

// Remove the nth node shield
void
dll_remove_nth_node_shield(galaxy_t ** list, unsigned int n);

// Remove a node from the nth position
void
dll_remove_nth_node(galaxy_t ** list, unsigned int n, int* global_size);

// Remove a node from the nth position
void
dll_remove_nth_node_implode(galaxy_t ** list, unsigned int n, int* global_size);

// Remove a node from the nth position
void
dll_remove_nth_node_p(galaxy_t ** list, unsigned int n);

// Get the size of the galaxy
unsigned int
dll_get_size(galaxy_t * list);

// Throw the galaxy into a black hole(release the memory)
void
dll_free(galaxy_t ** pp_list);

// Print the galaxy with int values
void
dll_print_int_list(galaxy_t * list);

// Print the galaxy with string values
void
dll_print_string_list(galaxy_t * list);

#endif
