/* Copyright Chelcea Claudiu-Marian */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "load_balancer.h"
#define STARTING_SERVERS 600
#define MAX_SERVER_ITEMS 1000

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
    my_load_balancer->hashring = malloc(STARTING_SERVERS * 3 * sizeof(struct server_pointer*));
    my_load_balancer->load_balancer_data = NULL;
    my_load_balancer->load_balancer_data = malloc(STARTING_SERVERS * sizeof(lb_data_t**));
    DIE(!my_load_balancer->hashring || !my_load_balancer->load_balancer_data, "Couldn't create load balancer!\n");

    // Allocate initial servers
    for(int i=0;i<my_load_balancer->num_servers;i++) {
        // Allocate three spaces in the hashring at a time
        my_load_balancer->hashring[i*3] = malloc(sizeof(struct server_pointer));
        my_load_balancer->hashring[i*3]->server_index = -1;
        my_load_balancer->hashring[i*3]->server_label = -1;
        my_load_balancer->hashring[i*3]->real_server_index = -1;
        my_load_balancer->hashring[i*3+1] = malloc(sizeof(struct server_pointer));
        my_load_balancer->hashring[i*3+1]->server_index = -1;
        my_load_balancer->hashring[i*3+1]->server_label = -1;
        my_load_balancer->hashring[i*3+1]->real_server_index = -1;
        my_load_balancer->hashring[i*3+2] = malloc(sizeof(struct server_pointer));
        my_load_balancer->hashring[i*3+2]->server_index = -1;
        my_load_balancer->hashring[i*3+2]->server_label = -1;
        my_load_balancer->hashring[i*3+2]->real_server_index = -1;
      
        // Allocate server's items
        my_load_balancer->load_balancer_data[i] = malloc(MAX_SERVER_ITEMS * sizeof(sizeof(lb_data_t*)));
        for(int j=0;j<MAX_SERVER_ITEMS;j++) {
            my_load_balancer->load_balancer_data[i][j] = malloc(sizeof(lb_data_t));
            strncpy(my_load_balancer->load_balancer_data[i][j]->server_items,"",1);
            strncpy(my_load_balancer->load_balancer_data[i][j]->server_keys,"",1);
        }
    }

    DIE(!my_load_balancer->hashring, "Couldn't allocate space for servers!\n");

    return my_load_balancer;
}

// Stores a product (key - ID, value - product name) on one of the
// available servers using Consistent Hashing.
void loader_store(load_balancer* main, char* key, char* value, int* server_id) {
    // Check for the existence of the load balancer
    if(!main)
        return;

    // Get hash value of the key
	unsigned int key_hash = hash_function_key(key);
   // printf("%u\n\n",key_hash);
    
    int found_server = -1;
    int found_server_hashring_index = -1;
    // Get the server to store the data on
    for(int i=0;i<main->current_hashring_items;i++) {
        if(key_hash <= hash_function_servers(&main->hashring[i]->server_label)) {
            found_server = main->hashring[i]->server_index;
            found_server_hashring_index = i;
            break;
        }
    }

    // If we didn't find any server,
    // by the circular logic we will put the element on the first server
    if(found_server == -1) {
        // Get value's index
        int value_index = key_hash % MAX_SERVER_ITEMS;
        for(int r = value_index; r < value_index + MAX_SERVER_ITEMS; r++) {
            int pos = r % MAX_SERVER_ITEMS;
            if(strcmp(main->load_balancer_data[main->hashring[0]->server_index][pos]->server_keys, "") == 0) {
                strncpy(main->load_balancer_data[main->hashring[0]->server_index][pos]->server_keys, key,strlen(key));
                strncpy(main->load_balancer_data[main->hashring[0]->server_index][pos]->server_items,value,strlen(value));
                break;
            }
        }
        // Add to first server
       // strncpy(main->load_balancer_data[0][value_index]->server_keys, key,strlen(key));
        //printf("%s %s %s %s\n",main->load_balancer_data[0][value_index]->server_keys,main->load_balancer_data[0][value_index + 4]->server_keys,main->load_balancer_data[0][value_index]->server_items,main->load_balancer_data[0][value_index + 4]->server_items);

        
        //printf("%s %s %s %s\n",main->load_balancer_data[0][value_index]->server_keys,main->load_balancer_data[0][value_index+4]->server_keys,main->load_balancer_data[0][value_index]->server_items,main->load_balancer_data[0][value_index + 4]->server_items);

        
        *server_id  = main->hashring[0]->real_server_index;
        
    }
    else {
        // Get value's index
        int value_index = key_hash % MAX_SERVER_ITEMS;
        for(int r = value_index; r < value_index + MAX_SERVER_ITEMS; r++) {
            int pos = r % MAX_SERVER_ITEMS;
            if(strcmp(main->load_balancer_data[found_server][pos]->server_keys, "") == 0) {
                strncpy(main->load_balancer_data[found_server][pos]->server_keys, key,strlen(key));
                strncpy(main->load_balancer_data[found_server][pos]->server_items,value,strlen(value));
                break;
            }
        }
        // Add to certain server
        *server_id  = main->hashring[found_server_hashring_index]->real_server_index;
    }
}

// Calculates on which server the key is stored and extracts its value.
char* loader_retrieve(load_balancer* main, char* key, int* server_id) {
	// Check for the existence of the load balancer
    if(!main)
        return NULL;

    // Get hash value of the key
	unsigned int key_hash = hash_function_key(key);

    int found_server_hashring_index = -1;
    *server_id = -1;
    // Get the server to store the data on
    for(int i=0;i<main->current_hashring_items;i++) {
        if(key_hash <= hash_function_servers(&main->hashring[i]->server_label)) {
            found_server_hashring_index = i;
            *server_id = main->hashring[i]->server_index;
            break;
        }
    }

    // If we didn't find any server,
    // by the circular logic we will search the first server
    if(*server_id == -1) {
        // Get value's index
        unsigned int value_index = key_hash % MAX_SERVER_ITEMS;
        *server_id  = main->hashring[0]->real_server_index;
        int max = value_index + MAX_SERVER_ITEMS;

        for(int r = value_index; r < max; r++) {
            int pos = r % MAX_SERVER_ITEMS;
            if(strcmp(main->load_balancer_data[main->hashring[0]->server_index][pos]->server_keys,key) == 0)
                return main->load_balancer_data[main->hashring[0]->server_index][pos]->server_items;
        }
        return NULL;
    }
    else {
    
        // Get value's index
        unsigned int value_index = key_hash % MAX_SERVER_ITEMS;
        int sv_index = *server_id;
        *server_id  = main->hashring[found_server_hashring_index]->real_server_index;
        int max = value_index + MAX_SERVER_ITEMS; 

        for(int r = value_index; r < max; r++) {
            int pos = r % MAX_SERVER_ITEMS;
            if(strcmp(main->load_balancer_data[sv_index][pos]->server_keys,key) == 0)
                return main->load_balancer_data[sv_index][pos]->server_items;
            }  
            return NULL;
    }
}

// Add server by label
void add_server_by_label(load_balancer* main, int server_label, int server_id, int server_index)
{
    // Create hash of label
    unsigned int label__hash = hash_function_servers(&server_label);

    // Search the current servers
    int index = -1;
    for(int i=0;i<main->current_hashring_items;i++) {
       // printf("%u %u %d\n",label__hash,(unsigned int)hash_function_servers(&main->hashring[i]->server_label),label__hash<(unsigned int)hash_function_servers(&main->hashring[i]->server_label));
        if((unsigned int)label__hash<=(unsigned int)hash_function_servers(&main->hashring[i]->server_label)) {
            index = i;
            break;
        }
    }

    // If equal elements, sort by ID
    if(index != -1 && label__hash == (unsigned int)hash_function_servers(&(main->hashring[index]->server_label))) {
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
            new_server->real_server_index = server_id;
            
            // Put it in the list
            main->hashring[main->current_hashring_items] = new_server;
            main->current_hashring_items = main->current_hashring_items + 1;
          
            // Rebalance (we 'steal' from the last element in the hashring and from the first)

            // LAST ELEMENT
            for(int j=0;j<MAX_SERVER_ITEMS;j++) {
                if(strcmp(main->load_balancer_data[main->hashring[main->current_hashring_items-1]->server_index][j]->server_items,"") !=0 ) {
                    unsigned int key_hash = hash_function_key(&main->load_balancer_data[main->hashring[main->current_hashring_items-1]->server_index][j]->server_keys);
                    if(key_hash < hash_function_servers(&server_label) && key_hash > hash_function_servers(&main->hashring[main->current_hashring_items-1]->server_label)) {
                       // apply linear probing
                        int start = key_hash % MAX_SERVER_ITEMS;
                        for(int r = start; r < start +  MAX_SERVER_ITEMS; r++) {
                            int pos = r % MAX_SERVER_ITEMS;
                            if(strcmp(main->load_balancer_data[server_index][pos]->server_items,"") ==0) {
                                strcpy(main->load_balancer_data[server_index][pos]->server_items,main->load_balancer_data[main->hashring[main->current_hashring_items-1]->server_index][j]->server_items);
                                strcpy(main->load_balancer_data[server_index][pos]->server_keys,main->load_balancer_data[main->hashring[main->current_hashring_items-1]->server_index][j]->server_keys);
                                strcpy(main->load_balancer_data[main->hashring[main->current_hashring_items-1]->server_index][j]->server_items,"");
                                strcpy(main->load_balancer_data[main->hashring[main->current_hashring_items-1]->server_index][j]->server_keys,"");
                                break;
                            }
                        }
                    } 
                }
            }

            // FIRST ELEMENT
            for(int j=0;j<MAX_SERVER_ITEMS;j++) {
                if(strcmp(main->load_balancer_data[main->hashring[0]->server_index][j]->server_items,"") !=0 ) {
                    unsigned int key_hash = hash_function_key(&main->load_balancer_data[main->hashring[0]->server_index][j]->server_keys);

                    if(key_hash < hash_function_servers(&server_label) && key_hash > hash_function_servers(&main->hashring[main->current_hashring_items-2]->server_label)) {
                       // apply linear probing
                        int start = key_hash % MAX_SERVER_ITEMS;
                        for(int r = start; r < start +  MAX_SERVER_ITEMS; r++) {
                            int pos = r % MAX_SERVER_ITEMS;
                            if( strcmp(main->load_balancer_data[server_index][pos]->server_items,"") ==0) {
                                strcpy(main->load_balancer_data[server_index][pos]->server_items,main->load_balancer_data[main->hashring[0]->server_index][j]->server_items);
                                strcpy(main->load_balancer_data[server_index][pos]->server_keys,main->load_balancer_data[main->hashring[0]->server_index][j]->server_keys);
                                strcpy(main->load_balancer_data[main->hashring[0]->server_index][j]->server_items,"");
                                strcpy(main->load_balancer_data[main->hashring[0]->server_index][j]->server_keys,"");
                                break;
                            }
                        }
                    } 
                }
            }
        }
        // Add to the beginning
        else if(main->hashring[0]->server_label == -1 || label__hash < (unsigned int)hash_function_servers(&main->hashring[0]->server_label)) {
            // Create new server_pointer
            server_pointer* new_server = NULL;
            new_server = malloc(sizeof(server_pointer));
            DIE(!new_server, "Couldn't create new server!\n");
            new_server->server_index = server_index;
            new_server->server_label = server_label;
            new_server->real_server_index = server_id;

            // Add first new_server;
            for(int i=main->current_hashring_items;i>=1;i--)
                main->hashring[i] = main->hashring[i-1];
            main->current_hashring_items = main->current_hashring_items + 1;
            main->hashring[0] = new_server;
            //printf("Move item\n\n");
        }
    }
    else { // Add in between the elements
        // Create new server_pointer
        server_pointer* new_server = NULL;
        new_server = malloc(sizeof(server_pointer));
        DIE(!new_server, "Couldn't create new server!\n");
        new_server->server_index = server_index;
        new_server->server_label = server_label;
        new_server->real_server_index = server_id;

        // Move all server pointers from index to the right
        for(int i = main->current_hashring_items; i > index; i--) {
            main->hashring[i] = main->hashring[i-1];
        }
        
        // Set the new server pointer
        main->hashring[index] = new_server;
        main->current_hashring_items = main->current_hashring_items + 1;
        //printf("%d %d\n\n\n",index,main->hashring[index+1]->server_index);
        // Rebalance
        for(int i=0;i < MAX_SERVER_ITEMS; i++) {
            // Get all items on the server
            if(strcmp(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items, "") !=0) {
              //  printf("Item %s on position %d on server %d.\n",main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items,i,main->hashring[index+1]->server_index);

                // Get the current server's hash
                unsigned int key_hash = hash_function_key(&main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys);
            
                // If key's hash is smaller than the next server and if it's bigger than the newly added server
                if(key_hash < label__hash) {
                 
                    if(index && key_hash > hash_function_servers(&main->hashring[index-1]->server_label)) {
                        // If the location is empty, fill it
                        if(strcmp(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_items,"") ==0) {
                            strcpy(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_items,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items);
                            strcpy(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_keys,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys);
                            strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items,"");
                            strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys,"");
                        } else { // else apply linear probing
                            int start = key_hash % MAX_SERVER_ITEMS;
                            for(int r=start;r<start +  MAX_SERVER_ITEMS; r++) {
                                int pos = r % MAX_SERVER_ITEMS;
                                if(strcmp(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_items,"") ==0) {
                                    strcpy(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_items,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items);
                                    strcpy(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_keys,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys);
                                    strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items,"");
                                    strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys,"");
                                    break;
                                }
                            }
                        }
                    }
                    else if(!index) {
                      //  printf("Compar %u cu %u prin %s\n",key_hash,label__hash,main->load_balancer_data[main->hashring[index]->server_index][i]->server_items);
                        
                        if(strcmp(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_items,"") ==0) {
                            strcpy(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_items,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items);
                            strcpy(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_keys,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys);
                            strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items,"");
                            strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys,"");
                        }
                        else { // else apply linear probing
                            int start = key_hash % MAX_SERVER_ITEMS;
                            for(int r=start;r<start +  MAX_SERVER_ITEMS; r++) {
                                int pos = r % MAX_SERVER_ITEMS;
                                if(strcmp(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_items,"") ==0) {
                                    strcpy(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_items,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items);
                                    strcpy(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_keys,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys);
                                    strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items,"");
                                    strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys,"");
                                    break;
                                }
                            }
                        }
                    }
                }
                else if(index == 0) {
                    int server_index_last = main->hashring[main->current_hashring_items-1]->server_label;
                    if(key_hash < label__hash || key_hash > hash_function_servers(&server_index_last)) {
                        if(strcmp(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_items,"") ==0) {
                                strcpy(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_items,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items);
                                strcpy(main->load_balancer_data[main->hashring[index]->server_index][key_hash % MAX_SERVER_ITEMS]->server_keys,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys);
                                strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items,"");
                                strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys,"");
                        } else { // else apply linear probing
                            int start = key_hash % MAX_SERVER_ITEMS;
                            for(int r=start;r<start +  MAX_SERVER_ITEMS; r++) {
                                int pos = r % MAX_SERVER_ITEMS;
                                if(strcmp(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_items,"") ==0) {
                                    strcpy(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_items,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items);
                                    strcpy(main->load_balancer_data[main->hashring[index]->server_index][pos]->server_keys,main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys);
                                    strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_items,"");
                                    strcpy(main->load_balancer_data[main->hashring[index+1]->server_index][i]->server_keys,"");
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int cmpfunc(const void * a, const void * b) {
    return *(int*)a - *(int*)b;
}

// Add a new server to the system and rebalance the objects.
void loader_add_server(load_balancer* main, int server_id) {
	DIE(!main, "Didn't receive load_balancer!\n");
    // Create labels
    int label_1 = REPLICA * 0 + server_id;
    int label_2 = REPLICA * 1 + server_id;
    int label_3 = REPLICA * 2 + server_id;
    
    int n =  main->current_hashring_items;
    int* indexes = calloc(n, sizeof(int));
    for(int i=0;i<main->current_hashring_items;i++) {
        indexes[i] = main->hashring[i]->server_index;
    }
    
    // Problem
    qsort(indexes, main->current_hashring_items, sizeof(int), cmpfunc);

    int my_server_id = -1;
    for(int i=0;i<main->current_hashring_items;i++) {
        if(i/3 != indexes[i]) {
            my_server_id = i/3;
            break;
        }
    }    // 0 0 0 2 2 2 3 3 3 4 4 4

    if(my_server_id == -1) {
        if(main->current_hashring_items > 0)
            my_server_id = main->current_hashring_items / 3;
        else
            my_server_id = 0;
    }

    int server_index = my_server_id;
    if(my_server_id == main->num_servers)
        fprintf(stderr,"Need to resize!\n"),exit(EXIT_FAILURE);
   // printf("SERVER HASH %d %u %u %u\n",server_id,hash_function_servers(&label_1),hash_function_servers(&label_2),hash_function_servers(&label_3));
    // Find the position
    add_server_by_label(main, label_1, server_id, server_index);
    add_server_by_label(main, label_2, server_id, server_index);
    add_server_by_label(main, label_3, server_id, server_index);
}

// Remove a server from the load balancer
void move_server_items(load_balancer* main, int index_replica_id, int server_id)
{   
    // Move all elements to the next server in the hash ring
    for(int i=0;i < MAX_SERVER_ITEMS; i++) {
        // Get all items on the server
        if(strcmp(main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_items, "") !=0) {
        // printf("Item to fucking move %s\n",main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_items);
            // Rellocate them on the next server
            int index_new_server = -1;
         
            // Get the current server's hash
            unsigned int key_hash = hash_function_key(&main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_keys);
           
            // Find the first server bigger than him
            for(int j=0;j<main->current_hashring_items;j++) {
                // printf("ASTA %x VS %x\n\n",key_hash,hash_function_servers(&main->hashring[j]->server_label));
                if(server_id == main->hashring[j]->real_server_index)
                    continue;
                //("Comparring %u with %u\n",key_hash, hash_function_servers(&main->hashring[j]->server_label));
                if(key_hash < hash_function_servers(&main->hashring[j]->server_label)) {
                    index_new_server = j;
                    break;
                }
            }
          //  printf("\n\n%d\n\n",index_new_server);
          //  printf("%u\n\n",key_hash);
           
            // If found is -1, by the circular logic we add it to the first server, if it exists
            // otherwise we add it to the requested server
            if(index_new_server == -1 && main->current_hashring_items / 3 > 0) {
                int start = key_hash % MAX_SERVER_ITEMS;
                for(int r=start; r< start + MAX_SERVER_ITEMS; r++) {
                    int pos = r % MAX_SERVER_ITEMS;
                    if(strcmp(main->load_balancer_data[main->hashring[0]->server_index][pos]->server_items, "") == 0) {
                        strcpy(main->load_balancer_data[main->hashring[0]->server_index][pos]->server_items,main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_items);
                        strcpy(main->load_balancer_data[main->hashring[0]->server_index][pos]->server_keys,main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_keys);
                        strcpy(main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_keys,"");
                        strcpy(main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_items,"");
                        break;
                    }
                }

                //printf("MOVED TO THE START\n");
            }
            else {
            //  printf("Moved to server %d\n", main->hashring[index_new_server]->server_index);
                int start = key_hash % MAX_SERVER_ITEMS;
                for(int r=start; r< start + MAX_SERVER_ITEMS; r++) {
                    int pos = r % MAX_SERVER_ITEMS;
                    if(strcmp(main->load_balancer_data[index_new_server][pos]->server_items, "") == 0) {
                        strcpy(main->load_balancer_data[main->hashring[index_new_server]->server_index][pos]->server_items,main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_items);
                        strcpy(main->load_balancer_data[main->hashring[index_new_server]->server_index][pos]->server_keys,main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_keys);
                        strcpy(main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_keys,"");
                        strcpy(main->load_balancer_data[main->hashring[index_replica_id]->server_index][i]->server_items,"");
                        break;
                    }
                }
            }
           // printf("%s\n",main->server_items[index_replica_id][i]);
        }
    }
}
// Remove a server from the load balancer
void destroy_replica(load_balancer* main, int index_replica_id)
{
  // Remove server
    for(int i = index_replica_id;i<main->current_hashring_items;i++)
        main->hashring[i] = main->hashring[i+1];

    main->current_hashring_items -=1;    
}

// Delete all replicas and the server
void delete_all_replicas(load_balancer* main, int index_replica_id_0, int index_replica_id_1, int index_replica_id_2, int server_id)
{
    if(index_replica_id_0 > index_replica_id_1 && index_replica_id_0 > index_replica_id_2 && index_replica_id_1 > index_replica_id_2) { // 3 2 1
        
        // Rebalance items and destroy replica
        move_server_items(main,index_replica_id_0,server_id);
        destroy_replica(main, index_replica_id_0);
        destroy_replica(main, index_replica_id_1);
        destroy_replica(main, index_replica_id_2);
   }
   else if(index_replica_id_0 > index_replica_id_1 && index_replica_id_0 > index_replica_id_2 && index_replica_id_1 < index_replica_id_2) { // 3 1 2
   // Rebalance items and destroy replica
        move_server_items(main,index_replica_id_0,server_id);
        destroy_replica(main, index_replica_id_0);
        destroy_replica(main, index_replica_id_2);
        destroy_replica(main, index_replica_id_1);
   }
   else if(index_replica_id_0 < index_replica_id_1 && index_replica_id_0 > index_replica_id_2 && index_replica_id_1 > index_replica_id_2) { // 2 3 1
   // Rebalance items and destroy replica
        move_server_items(main,index_replica_id_1,server_id);
        destroy_replica(main, index_replica_id_1);
        destroy_replica(main, index_replica_id_0);
        destroy_replica(main, index_replica_id_2);
   }
   else if(index_replica_id_0 < index_replica_id_2 && index_replica_id_0 > index_replica_id_1 && index_replica_id_1 < index_replica_id_2) { // 2 1 3
   // Rebalance items and destroy replica
        move_server_items(main,index_replica_id_2,server_id);
        destroy_replica(main, index_replica_id_2);
        destroy_replica(main, index_replica_id_0);
        destroy_replica(main, index_replica_id_1);
   }
   else if(index_replica_id_0 < index_replica_id_1 && index_replica_id_0 < index_replica_id_2 && index_replica_id_1 > index_replica_id_2) { // 1 3 2
   // Rebalance items and destroy replica
        move_server_items(main,index_replica_id_1,server_id);
        destroy_replica(main, index_replica_id_1);
        destroy_replica(main, index_replica_id_2);
        destroy_replica(main, index_replica_id_0);
   }
   else if(index_replica_id_0 < index_replica_id_1 && index_replica_id_0 < index_replica_id_2 && index_replica_id_1 < index_replica_id_2) { // 1 2 3
        // Rebalance items and destroy replica
        move_server_items(main,index_replica_id_2,server_id);
        destroy_replica(main, index_replica_id_2);
        destroy_replica(main, index_replica_id_1);
        destroy_replica(main, index_replica_id_0);
   }
}

// Removes a server from the system and rebalances the objects.
void loader_remove_server(load_balancer* main, int server_id) {
	// Check for the existence of the load balancer
    if(!main)
        return;

    // Get server indexes in hashring
    int index_replica_id_0  = -1;
    int index_replica_id_1  = -1;
    int index_replica_id_2  = -1;
    for(int i=0;i<main->current_hashring_items;i++) {
        if(server_id == main->hashring[i]->real_server_index) {
            // We will find tree indexes, since this is the the saving method
            // a server + 2 replicas
            if(index_replica_id_0 == -1)
                index_replica_id_0 = i;
            else if(index_replica_id_1 == -1)
                index_replica_id_1 = i;
            else if(index_replica_id_2 == -1)
                index_replica_id_2 = i;
            else
                break;
        }
    }
 
   delete_all_replicas(main,index_replica_id_0,index_replica_id_1,index_replica_id_2, server_id);
}

// Print a server's items
void print_server(load_balancer* main, int server_id)
{
    if(!main->load_balancer_data[server_id])
        return;

    // Get all items
    for(int i=0;i<MAX_SERVER_ITEMS;i++) {
        //printf("%i %s\n",i,main->load_balancer_data[server_id][i]->server_items);
        // If item exists, print it
        int is_string = 1;
        if(strcmp(main->load_balancer_data[server_id][i]->server_items, "") == 0)
            is_string = -1;
        else {
            char compare_item = ' ';
            for(size_t j=0;j<strlen(main->load_balancer_data[server_id][i]->server_items);j++) {
                if(main->load_balancer_data[server_id][i]->server_items[0] == compare_item) {
                    is_string = -1;
                    break;
                }
            }
        }
        if(is_string == 1)
            printf("On position %d: %s on server %d.\n",i,main->load_balancer_data[server_id][i]->server_items,server_id);
    }
}

// Free memory
void free_load_balancer(load_balancer* my_load_balancer) {

    for(int i=0;i<my_load_balancer->current_hashring_items/3;i++) {
        free(my_load_balancer->hashring[i*3]);
        free(my_load_balancer->hashring[i*3+1]);
        free(my_load_balancer->hashring[i*3+2]);
        my_load_balancer->hashring[i*3] = NULL;
        my_load_balancer->hashring[i*3+1] = NULL;
        my_load_balancer->hashring[i*3+2] = NULL;
        for(int j=0;j<MAX_SERVER_ITEMS;j++) {
           free(my_load_balancer->load_balancer_data[i][j]);
           my_load_balancer->load_balancer_data[i][j] = NULL;
        }
        free(my_load_balancer->load_balancer_data[i]);
        my_load_balancer->load_balancer_data[i] = NULL;
    }

    free(my_load_balancer->load_balancer_data);
    my_load_balancer->load_balancer_data = NULL;
    free(my_load_balancer->hashring);   
    my_load_balancer->hashring = NULL; 

    free(my_load_balancer);
    my_load_balancer = NULL;
}
