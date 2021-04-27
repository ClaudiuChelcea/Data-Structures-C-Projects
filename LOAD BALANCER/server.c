/* Copyright Chelcea Claudiu-Marian */

#include <stdlib.h>
#include <string.h>
#include "server.h"

server_memory * init_server_memory() {
    // The server
    server_memory * my_server = NULL;
    my_server = malloc(sizeof(server_memory_t));
    return my_server;
}

// Stores data received from Load Balancer in a Hashtable.
void server_store(server_memory * server, char * key, char * value) {
    if (!server)
        return;
    strncpy(server -> server_keys, key, strlen(key));
    strncpy(server -> server_items, value, strlen(value));
}

// Returns the value associated with the key in the Hashtable.
void server_remove(server_memory * server, char * key) {
    if (!server || !key)
        return;
    strcpy(server -> server_keys, "");
    strcpy(server -> server_items, "");
}

// Delete an entry from the Hashtable.
char * server_retrieve(server_memory * server, char * key) {
	
    if (!server || !key)
        return NULL;
    if (strcmp(server -> server_keys, key) == 0)
        return server -> server_items;
    return NULL;
}

// Free memory
void free_server_memory(server_memory * server) {
    if (!server)
        return;

    free(server);
}
