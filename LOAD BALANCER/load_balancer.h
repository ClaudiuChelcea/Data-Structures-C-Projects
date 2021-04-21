/* Copyright Chelcea Claudiu-Marian */
#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_
#define REQUEST_LENGTH 1024
#define KEY_LENGTH 128
#define VALUE_LENGTH 65536
#define REPLICA 100000

#include "utils.h"
#include "server.h"
#include "client.h"
#include "linkedlist.h"

typedef struct lb_data
{
    char server_items[VALUE_LENGTH];
    char server_keys[VALUE_LENGTH];
} lb_data_t;


typedef struct load_balancer
{
    server_pointer** hashring;
    int num_servers;
    int current_hashring_items;
    lb_data_t*** load_balancer_data;
} load_balancer_t;


typedef struct load_balancer load_balancer;

load_balancer* init_load_balancer();
unsigned int hash_function_servers(void *a);
unsigned int hash_function_key(void *a);

// Print a server's items
void print_server(load_balancer* main, int server_id);

void free_load_balancer(load_balancer* main);

/**
 * load_store() - Stores the key-value pair inside the system.
 * @arg1: Load balancer which distributes the work.
 * @arg2: Key represented as a string.
 * @arg3: Value represented as a string.
 * @arg4: This function will RETURN via this parameter 
 *        the server ID which stores the object.
 *
 * The load balancer will use Consistent Hashing to distribute the 
 * load across the servers. The chosen server ID will be returned 
 * using the last parameter.
 */
void loader_store(load_balancer* main, char* key, char* value, int* server_id);

/**
 * load_retrieve() - Gets a value associated with the key.
 * @arg1: Load balancer which distributes the work.
 * @arg2: Key represented as a string.
 * @arg3: This function will RETURN the server ID 
          which stores the value via this parameter.
 *
 * The load balancer will search for the server which should posess the 
 * value associated to the key. The server will return NULL in case 
 * the key does NOT exist in the system.
 */
char* loader_retrieve(load_balancer* main, char* key, int* server_id);

/**
 * load_add_server() - Adds a new server to the system.
 * @arg1: Load balancer which distributes the work.
 * @arg2: ID of the new server.
 *
 * The load balancer will generate 3 replica TAGs and it will
 * place them inside the hash ring. The neighbor servers will 
 * distribute some the objects to the added server.
 */
void loader_add_server(load_balancer* main, int server_id);

/**
 * load_remove_server() - Removes a specific server from the system.
 * @arg1: Load balancer which distributes the work.
 * @arg2: ID of the removed server.
 *
 * The load balancer will distribute ALL objects stored on the
 * removed server and will delete ALL replicas from the hash ring.
 */
void loader_remove_server(load_balancer* main, int server_id);


#endif  /* LOAD_BALANCER_H_ */
