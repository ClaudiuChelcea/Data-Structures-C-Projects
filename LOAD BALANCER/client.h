/* Copyright Chelcea Claudiu-Marian */

#ifndef CHECKER_CLIENT_H_
#define CHECKER_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Execute command based on input
void apply_requests(FILE *input_file);

// Get element key from request
void get_key(char *key, char *request);

// Get element value from request
void get_key_value(char *key, char *value, char *request);

#endif  // CHECKER_CLIENT_H_
