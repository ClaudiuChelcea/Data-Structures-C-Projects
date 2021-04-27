/* Copyright Chelcea Claudiu-Marian */
#ifndef SERVER_H_
#define SERVER_H_
#define MAX_SERVER_ITEMS 1000
#define VALUE_LENGTH 400

// Value stored on the hashring that holds some data about a server
// It represents a server replica
typedef struct server_pointer
{
	int server_index;
	int server_label;
	int real_server_index;
} server_pointer;

// Memory for each object on the server
// The server is a hashtable that store this server_memory objects
// based on the server_keys field
struct server_memory
{
	char server_items[VALUE_LENGTH];
	char server_keys[VALUE_LENGTH];
};

typedef struct server_memory server_memory_t;
typedef struct server_memory server_memory;

// Initialise server's_memory
server_memory *init_server_memory();

// Release server's memory
void free_server_memory(server_memory *server);

/**
 * server_store() - Stores a key-value pair to the server.
 * @arg1: Server which performs the task.
 * @arg2: Key represented as a string.
 * @arg3: Value represented as a string.
 */
void server_store(server_memory *server, char *key, char *value);

/**
 * server_remove() - Removes a key-pair value from the server.
 * @arg1: Server which performs the task.
 * @arg2: Key represented as a string.
 */
void server_remove(server_memory *server, char *key);

/**
 * server_remove() - Gets the value associated with the key.
 * @arg1: Server which performs the task.
 * @arg2: Key represented as a string.
 *
 * Return: String value associated with the key
 *         or NULL (in case the key does not exist).
 */
char *server_retrieve(server_memory *server, char *key);

#endif  // SERVER_H_
