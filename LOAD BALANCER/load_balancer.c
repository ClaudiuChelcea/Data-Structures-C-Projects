/* Copyright Chelcea Claudiu-Marian */
#include <stdlib.h>
#include <string.h>
#include "load_balancer.h"
#define STARTING_SERVERS 10
#define MAX_SERVER_ITEMS 100
#define MAX_SERVER_OBJECT 64

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
    // Create load balancer
	load_balancer* my_load_balancer = NULL;
    my_load_balancer = malloc(sizeof(load_balancer));
    DIE(!my_load_balancer,"Couldn't create load balancer!\n");

    // Save data
    my_load_balancer->num_servers = STARTING_SERVERS;
    my_load_balancer->current_hashring_items = 0;
    my_load_balancer->hashring = NULL;
    my_load_balancer->hashring = malloc(STARTING_SERVERS * sizeof(struct server_pointer*));
    my_load_balancer->server_items = NULL;
    my_load_balancer->server_items = malloc(STARTING_SERVERS * sizeof(char**));
    DIE(!my_load_balancer->hashring || !my_load_balancer->server_items, "Couldn't create load balancer!\n");

    // Allocate initial servers
    for(int i=0;i<my_load_balancer->num_servers;i++) {
        my_load_balancer->hashring[i] = malloc(sizeof(struct server_pointer));
        my_load_balancer->hashring[i]->server_index = -1;
        my_load_balancer->hashring[i]->server_label = -1;
        my_load_balancer->server_items[i] = malloc(sizeof(MAX_SERVER_ITEMS * sizeof(char*)));
        for(int j=0;j<MAX_SERVER_ITEMS;j++) {
            my_load_balancer->server_items[i][j] = malloc(MAX_SERVER_OBJECT * sizeof(char));
        }
    }
    DIE(!my_load_balancer->hashring, "Couldn't allocate space for servers!\n");

    return my_load_balancer;
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

// Add server by label
void add_server_by_label(load_balancer* main, int server_label, int server_id, int server_index)
{
    // Create hash of labels
    unsigned int label__hash = hash_function_servers(&server_label);

    // Search the current servers
    int index = -1;
    for(int i=0;i<main->current_hashring_items;i++) { // UNSIGNED INT NOT COMPARING CORRECTLY
        // printf("%u %u %d\n",label__hash,(unsigned int)hash_function_servers(&main->hashring[i]->server_label),label__hash<(unsigned int)hash_function_servers(&main->hashring[i]->server_label));
        if((unsigned int)label__hash<=(unsigned int)hash_function_servers(&main->hashring[i]->server_label)) {
            index = i;
            break;
        }
    }

    // If equal elements, sort by ID
    if(index != -1 && main->hashring[index]->server_index != -1 && label__hash == (unsigned int)hash_function_servers(&(main->hashring[index]->server_label))) {
        if(server_id > main->hashring[index]->server_index)
            ++index;
    }

    // Check all cases
    if(index == -1) {
        // Add to the end
        if(main->current_hashring_items>0 && main->hashring[main->current_hashring_items-1]->server_label != -1 && label__hash > (unsigned int)hash_function_servers(&main->hashring[main->current_hashring_items-1]->server_label)) {
            // Create new server_pointer
            server_pointer* new_server = NULL;
            new_server = malloc(sizeof(server_pointer));
            DIE(!new_server, "Couldn't create new server!\n");
            new_server->server_index = server_index;
            new_server->server_label = server_label;

            // Put it in the list
            main->hashring[main->current_hashring_items] = new_server;
            main->current_hashring_items = main->current_hashring_items + 1;

            // Rebalance
        }
        // Add to the beginning
        else if(main->hashring[0]->server_label == -1 || label__hash < (unsigned int)hash_function_servers(&main->hashring[0]->server_label)) {
            // Create new server_pointer
            server_pointer* new_server = NULL;
            new_server = malloc(sizeof(server_pointer));
            DIE(!new_server, "Couldn't create new server!\n");
            new_server->server_index = server_index;
            new_server->server_label = server_label;

            // Add first new_server;
            for(int i=main->current_hashring_items;i>=1;i--)
                main->hashring[i] = main->hashring[i-1];
            main->current_hashring_items = main->current_hashring_items + 1;
            main->hashring[0] = new_server;
        }
    }
    else { // Add in between the elements
        // Create new server_pointer
        server_pointer* new_server = NULL;
        new_server = malloc(sizeof(server_pointer));
        DIE(!new_server, "Couldn't create new server!\n");
        new_server->server_index = server_index;
        new_server->server_label = server_label;

        // Move all server pointers from index to the right
        for(int i = main->current_hashring_items; i > index; i--) {
            main->hashring[i] = main->hashring[i-1];
        }
        
        // Set the new server pointer
        main->hashring[index] = new_server;
        main->current_hashring_items = main->current_hashring_items + 1;
    }
}

// Add a new server to the system and rebalance the objects.
void loader_add_server(load_balancer* main, int server_id) {
	DIE(!main, "Didn't receive load_balancer!\n");
    // Create labels
    int label_1 = REPLICA * 0 + server_id;
    int label_2 = REPLICA * 1 + server_id;
    int label_3 = REPLICA * 2 + server_id;
    
    // Choose server
    static int server_index = 0;
   
    // Find the position
    add_server_by_label(main,label_1, server_id, server_index);
    add_server_by_label(main,label_2, server_id, server_index);
    add_server_by_label(main,label_3, server_id, server_index);
    
    // Add the corresponding server
    ++server_index;
}

// Removes a server from the system and rebalances the objects.
void loader_remove_server(load_balancer* main, int server_id) {
	/* TODO. */
}

// Free memory
void free_load_balancer(load_balancer* main) {
    /* TODO. */
}
