/* Copyright Chelcea Claudiu-Marian */
#include <stdlib.h>
#include <string.h>
#include "load_balancer.h"

unsigned int hash_function_servers(void *a) {
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

unsigned int hash_function_key(void *a) {
    unsigned char *puchar_a = (unsigned char *) a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}


load_balancer* init_load_balancer() {
	/* TODO. */
    return NULL;
}

// Stores a product (key - ID, value - product name) on one of the
// available servers using Consistent Hashing.
void loader_store(load_balancer* main, char* key, char* value, int* server_id) {
	/* TODO. */
}

// Calculates on which server the key is stored and extracts its value.
char* loader_retrieve(load_balancer* main, char* key, int* server_id) {
	/* TODO. */
	return NULL;
}

// Add a new server to the system and rebalance the objects.
void loader_add_server(load_balancer* main, int server_id) {
	/* TODO. */
}

// Removes a server from the system and rebalances the objects.
void loader_remove_server(load_balancer* main, int server_id) {
	/* TODO. */
}

// Free memory
void free_load_balancer(load_balancer* main) {
    /* TODO. */
}
