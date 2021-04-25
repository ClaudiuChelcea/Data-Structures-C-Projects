/* Copyright Chelcea Claudiu-Marian */
#include <stdlib.h>
#include <string.h>
#include "server.h"

server_memory* init_server_memory() {
	/* TODO. */
	return NULL;
}

// Stores data received from Load Balancer in a Hashtable.
void server_store(server_memory* server, char* key, char* value) {
	/* TODO. */
}

// Returns the value associated with the key in the Hashtable.
void server_remove(server_memory* server, char* key) {
	/* TODO. */
}

// Delete an entry from the Hashtable.
char* server_retrieve(server_memory* server, char* key) {
	/* TODO. */
	return NULL;
}

// Free memory
void free_server_memory(server_memory* server) {
	/* TODO. */
}
