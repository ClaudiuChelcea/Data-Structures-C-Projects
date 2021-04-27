/* Copyright Chelcea Claudiu-Marian */ 

#include "client.h"
#include "load_balancer.h"
#include <ctype.h>
#define MAX_SERVER_ITEMS 1000

// Get element value from request
void get_key_value(char * key, char * value, char * request)
{
    char compare_item = '"';
    int second = 0;
    size_t index = 0;

    // Scan the request
    for (size_t i = 0; i < strlen(request); i++) {
        if (request[i] == compare_item) {
            i++;
            // If we find the first "", start searching untill the next one
            while (second != 1) {
                if (request[i++] == compare_item)
                    second++;
                // When we find the first one, we break out of the loop
                // and save the index
                if (second == 1)
                    break;
            }
            index = i;
            break;
        }
    }

    // Save the key in key
    // The command will be: store "key" "value"
    // So we start at index 7 and stop right before " to save the key
    int cnt = 0;
    for (size_t i = 7; i < index - 1; i++)
        key[cnt++] = request[i];
    cnt = 0;

    // We skip the " at index by adding +1 and skip the " and the string terminator
    // by substracting 2 on strlen(request)
    for (size_t i = index + 1; i < strlen(request) - 2; i++)
        value[cnt++] = request[i + 1];
}

// Get element's key from request
void get_key(char * key, char * request)
{
    int key_start = 0, key_index = 0;
    for (unsigned int i = 0; i < strlen(request); ++i) {
        if (request[i] == '"') {
            key_start = 1;
        } else if (key_start == 1) {
            key[key_index++] = request[i];
        }
    }
}

// Execute command based on input
void apply_requests(FILE * input_file)
{
	// Initialise values
    char request[REQUEST_LENGTH] = {0};
    char key[KEY_LENGTH] = {0};
    char value[VALUE_LENGTH] = {0};
    load_balancer * main_server = NULL;
    main_server = init_load_balancer();

    // Scan command
    while (fgets(request, REQUEST_LENGTH, input_file)) {
        request[strlen(request) - 1] = 0;
        static int i = 0;
        ++i;

        // Store item
        if (!strncmp(request, "store", sizeof("store") - 1)) {
			int index_server = 0;
            get_key_value(key, value, request);
            
            loader_store(main_server, key, value, & index_server);
            printf("Stored %s on server %d.\n", value, index_server);

			// Release key and value
            memset(key, 0, sizeof(key));
            memset(value, 0, sizeof(value));

            // Retrive item from server
        } else if (!strncmp(request, "retrieve", sizeof("retrieve") - 1)) {
			int index_server = 0;
            char * retrieved_value = NULL;
            get_key(key, request);
            
            retrieved_value = loader_retrieve(main_server, key, & index_server);
			
            if (retrieved_value) {
                printf("Retrieved %s from server %d.\n", retrieved_value, index_server);
            } else {
                printf("Key %s not present.\n", key);
            }

			// Release key
            memset(key, 0, sizeof(key));

            // Add new server
        } else if (!strncmp(request, "add_server", sizeof("add_server") - 1)) {
            int server_id = atoi(request + sizeof("add_server"));
            loader_add_server(main_server, server_id);

            // Remove server
        } else if (!strncmp(request, "remove_server",
                sizeof("remove_server") - 1)) {
            int server_id = atoi(request + sizeof("remove_server"));

            loader_remove_server(main_server, server_id);

        } else {
            DIE(1, "unknown function call");
        }
    }

    // Release memory
    free_load_balancer(main_server);
}
