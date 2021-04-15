/* Copyright Chelcea Claudiu-Marian */
#ifndef __CLIENT__H_
#define __CLIENT__H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Stores a product (a value) in the system and gives it an ID (key)
void client_store(char* key, char* value);

// Return the name of the product associated with an ID
void client_retrieve(char* key);

// Execute command based on input
void apply_requests(FILE* input_file);

// Get element key from request
void get_key(char* key, char* request);

// Get element value from request
void get_key_value(char* key, char* value, char* request);

#endif