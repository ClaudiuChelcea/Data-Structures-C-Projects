/* Copyright Chelcea Claudiu-Marian */

#ifndef __CLIENT__H_
#define __CLIENT__H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Execute command based on input
void apply_requests(FILE * input_file);

// Get element key from request
void get_key(char * key, char * request);

// Get element value from request
void get_key_value(char * key, char * value, char * request);

#endif
