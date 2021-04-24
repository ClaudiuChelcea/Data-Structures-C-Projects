/* Copyright Chelcea Claudiu-Marian */
#include "client.h"
#include "load_balancer.h"
#include <ctype.h>
#define MAX_SERVER_ITEMS 1000

int debug = 0;

// Get element value from request
void get_key_value(char* key, char* value, char* request)
{
	char compare_item = '"';
	int second = 0;
	size_t index = 0;
	
	// Scan the request
	for(size_t i=0;i<strlen(request);i++) {
		if(request[i] == compare_item) {
			i++;
			// If we find the first "", start searching untill the next one
			while(second != 1) {
				if(request[i++] == compare_item)
					second++;
				// When we find the first one, we break out of the loop
				// and save the index
				if(second == 1)
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
	for(size_t i = 7;i<index-1;i++)
		key[cnt++] = request[i];
	cnt = 0;
	
	// We skip the " at index by adding +1 and skip the " and the string terminator
	// by substracting 2 on strlen(request)
	for(size_t i = index+1;i<strlen(request)-2;i++)
		value[cnt++] = request[i+1];
}

// Get element key from request
void get_key(char* key, char* request)
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
void apply_requests(FILE* input_file)
{
	char request[REQUEST_LENGTH] = {0};
	char key[KEY_LENGTH] = {0};
	char value[VALUE_LENGTH] = {0};
	load_balancer* main_server = NULL;
	main_server = init_load_balancer();

	// Scan command
	while (fgets(request, REQUEST_LENGTH, input_file)) {
		request[strlen(request) - 1] = 0;
		static int i = 0;
		++i;

		// Store item
		if (!strncmp(request, "store", sizeof("store") - 1)) {
			get_key_value(key, value, request);
			int index_server = 0;
			loader_store(main_server, key, value, &index_server);
			if(debug)
				printf("Hash for %s is %u.-----------\n",value,hash_function_key(key));
			printf("Stored %s on server %d.\n", value, index_server);

			memset(key, 0, sizeof(key));
			memset(value, 0, sizeof(value));

		// Return item
		} else if (!strncmp(request, "retrieve", sizeof("retrieve") - 1)) {
			get_key(key, request);

			int index_server = 0;
			char *retrieved_value = NULL;
			retrieved_value = loader_retrieve(main_server,
											key, &index_server);
			if (retrieved_value) {
				printf("Retrieved %s from server %d.\n",
						retrieved_value, index_server);
			} else {
				printf("Key %s not present.\n", key);
			}

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
	if(debug) {
		for(int i=0;i<main_server->current_hashring_items;i++) {
			printf("Server real index: %d index %d label %d hash %u\n",main_server->hashring[i]->real_server_index,main_server->hashring[i]->server_index,main_server->hashring[i]->server_label,hash_function_servers(&main_server->hashring[i]->server_label));
		}
	}
	if(debug) {
		print_server(main_server,0);
		print_server(main_server,1);
		print_server(main_server,2);
		print_server(main_server,3);
		print_server(main_server,4);
		print_server(main_server,5);
		print_server(main_server,6);
		print_server(main_server,7);
	}

	// Release memory
	free_load_balancer(main_server);
}
