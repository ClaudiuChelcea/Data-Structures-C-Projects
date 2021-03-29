#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Galaxy.h"

// Allocate list
galaxy_t * dll_create() {
    galaxy_t * new = NULL;
    new = malloc(sizeof(galaxy_t));
    DIE(!new, "Couldn't create list!\n");
    new -> head = NULL;
    new -> galaxy_size = 0;
    return new;
}

// Return the nth node
galaxy_object * dll_get_nth_node(galaxy_t * list, unsigned int n)
{
    // Check the existence of the node
    if (!list) {
        printf("No list!\n");
        return NULL;
    } else if (!list -> head) {
        printf("List is empty!\n");
        return NULL;
    } else if (n < 0) {
        printf("Invalid index!\n");
        return NULL;
    } else if (n == 0) { // if index == 0
        return list -> head;
    } else if (n >= list -> galaxy_size) { // If index > the number of planets
        return list -> head -> prev;
    } else { // The element at the specified index
        galaxy_object * node = NULL;
        node = list -> head -> prev;
        if (!node) {
            printf("Couldn't remove nth node!\n");
            return NULL;
        }
        while (n) {
            node = node -> next;
            n--;
        }
        return node;
    }
}

// Add node to nth position
void dll_add_nth_node_shield(galaxy_t * list, unsigned int n,
                             const void * data)
{
    // Check the existence of the list
    if (!list) {
        printf("No list to add node to!\n");
        return;
    } else if (n < 0) {
        printf("Invalid index!\n");
        return;
    } else if (!list -> head) { // If the list's head is empty, create it
        galaxy_object * new_node = NULL;
        new_node = malloc(sizeof(galaxy_object));
        new_node -> next = new_node;
        new_node -> prev = new_node;
        new_node -> data = malloc(sizeof(int));
        *((int * ) new_node -> data) = * ((int * ) data);
        list -> head = new_node;
        list -> galaxy_size++;
    } else if (n == 0) { // Create shield as first shield in the list
        galaxy_object * new_node = NULL;
        new_node = malloc(sizeof(galaxy_object));
        DIE(!new_node, "Couldn't add first item to list!\n");

        new_node -> next = NULL;
        new_node -> prev = NULL;
        new_node -> data = malloc(sizeof(int));

        *((int * ) new_node -> data) = * ((int * ) data);

        new_node -> next = list -> head;
        new_node -> prev = list -> head -> prev;
        list -> head -> prev -> next = new_node;
        list -> head -> prev = new_node;
        list -> head = new_node;
        list -> galaxy_size++;

    } else if (n >= list -> galaxy_size) { // Create shield as last item
        galaxy_object * new_node = NULL;
        new_node = malloc(sizeof(galaxy_object));
        DIE(!new_node, "Couldn't add to list!\n");
        new_node -> next = NULL;
        new_node -> prev = NULL;
        new_node -> data = malloc(sizeof(int));
        *((int * ) new_node -> data) = * ((int * ) data);
        list -> galaxy_size++;
        new_node -> prev = list -> head -> prev;
        list -> head -> prev -> next = new_node;
        new_node -> next = list -> head;
        list -> head -> prev = new_node;

    } else { // Add shield at an index
        list -> galaxy_size++;
        galaxy_object * new_node_add = NULL;
        new_node_add = dll_get_nth_node(list, n);
        new_node_add = new_node_add -> next;

        galaxy_object * new_node = NULL;
        new_node = malloc(sizeof(galaxy_object));
        DIE(!new_node, "Couldn't add to list!\n");
        new_node -> next = NULL;
        new_node -> prev = NULL;
        new_node -> data = malloc(sizeof(int));
        *((int * ) new_node -> data) = * ((int * ) data);

        new_node -> next = new_node_add;
        new_node -> prev = new_node_add -> prev;
        new_node_add -> prev -> next = new_node;
        new_node_add -> prev = new_node;
    }
}

// Add node to nth position
void dll_add_nth_node(galaxy_t ** list, unsigned int planet_index,
                      int shields_number, char * planet_name, int* global_size)
{
    // Check the index
    if(planet_index < 0) {
        printf("Planet out of bounds!\n");
        return;
    }
    else if (!( * list)) {
        // Create the list
        ( * list) = dll_create();
        ( * list) -> galaxy_size = 1;

        // Create the head of the list
        galaxy_object * new_object = NULL;
        new_object = malloc(sizeof(galaxy_object));
        DIE(!new_object, "Couldn't chain planets!\n");
        
        // Allocate the head
        new_object -> next = new_object;
        new_object -> prev = new_object;
        new_object -> data = NULL;
        new_object->data = malloc(sizeof(only_data_t));
        DIE(!((only_data_t*)(new_object->data)), "Couldn't chain planets!\n");

        // Save the data of the head
        ((only_data_t*)(new_object->data)) -> shields_number = shields_number;
        ((only_data_t*)(new_object->data)) -> destroyed_planets = 0;
        ((only_data_t*)(new_object->data)) -> name = NULL;
        ((only_data_t*)(new_object->data)) -> name = malloc(NAME_SIZE * sizeof(char));
        DIE(!((only_data_t*)(new_object->data)) -> name, "Couldn't chain planets!\n");
        strcpy(((only_data_t*)(new_object->data)) -> name, planet_name);

        // Create the shield list
        galaxy_t * shield = NULL;
        shield = dll_create();
        DIE(!shield, "Couldn't create planet's shields!");
        int tmp = 1;
        for (int i = 0; i < shields_number; i++)
            dll_add_nth_node_shield(shield, 0, & tmp);
        ((only_data_t*)(new_object->data)) -> shield = shield;

        printf("The planet %s has joined the galaxy.\n", ( ((only_data_t*)(new_object->data))-> name));
        *global_size = *global_size + 1;
        ( * list) -> head = new_object;
        return;
        
    } else if (!( * list) -> head) { // If the list doesn't have a head

        (* list) -> galaxy_size = 1;
        // Create the head of the list
        galaxy_object * new_object = NULL;
        new_object = malloc(sizeof(galaxy_object));
        DIE(!new_object, "Couldn't chain planets!\n");
       
        // Allocate the head
        new_object -> next = new_object;
        new_object -> prev = new_object;
        new_object -> data = NULL;
        new_object -> data = malloc(sizeof(only_data_t));
        DIE(!new_object->data, "Couldn't chain planets!\n");

        // Save the data of the head
        ((only_data_t*)(new_object->data))->shields_number = shields_number;
        ((only_data_t*)(new_object->data)) -> destroyed_planets = 0;
        ((only_data_t*)(new_object->data)) -> name = NULL;
        ((only_data_t*)(new_object->data)) -> name = malloc(NAME_SIZE * sizeof(char));
        DIE(!((only_data_t*)(new_object->data)) -> name, "Couldn't chain planets!\n");
        strcpy( ((only_data_t*)(new_object->data)) -> name, planet_name);

        // Create the shield list
        galaxy_t * shield = NULL;
        shield = dll_create();
        DIE(!shield, "Couldn't create planet's shields!");
        int tmp = 1;
        for (int i = 0; i < shields_number; i++)
            dll_add_nth_node_shield(shield, 0, & tmp);
        ((only_data_t*)(new_object->data)) -> shield = shield;

        printf("The planet %s has joined the galaxy.\n", ( ((only_data_t*)(new_object->data)) -> name));
        *global_size = *global_size + 1;
         ( * list) -> head = new_object;

    } else if (planet_index == 0) { // If we add it as the first item
        // Create the head of the list
        galaxy_object * new_object = NULL;
        new_object = malloc(sizeof(galaxy_object));
        DIE(!new_object, "Couldn't chain planets!\n");

        // Allocate the head
        new_object -> next = NULL;
        new_object -> prev = NULL;
        new_object -> data = NULL;
        new_object->data = malloc(sizeof(only_data_t));
        DIE(!((only_data_t*)(new_object->data)), "Couldn't chain planets!\n");

        // Save the data of the head
        ((only_data_t*)(new_object->data)) -> shields_number = shields_number;
        ((only_data_t*)(new_object->data)) -> destroyed_planets = 0;
        ((only_data_t*)(new_object->data)) -> name = NULL;
        ((only_data_t*)(new_object->data)) -> name = malloc(NAME_SIZE * sizeof(char));
        DIE(! ((only_data_t*)(new_object->data)) -> name, "Couldn't chain planets!\n");
        strcpy( ((only_data_t*)(new_object->data)) -> name, planet_name);

        // Create the shield list
        galaxy_t * shield = NULL;
        shield = dll_create();
        DIE(!shield, "Couldn't create planet's shields!");
        int tmp = 1;
        for (int i = 0; i < shields_number; i++)
            dll_add_nth_node_shield(shield, 0, & tmp);
        ((only_data_t*)(new_object->data)) -> shield = shield;

        // Put the new node in the list
        new_object -> next = ( * list) -> head;
        new_object -> prev = ( * list) -> head -> prev;
        ( * list) -> head -> prev -> next = new_object;
        ( * list) -> head -> prev = new_object;
        ( * list) -> head = new_object;
        ( * list) -> galaxy_size++;
        printf("The planet %s has joined the galaxy.\n", (((only_data_t*)(new_object->data)) -> name));
        *global_size = *global_size + 1;

    } else if ((unsigned int) planet_index >= ( * list) -> galaxy_size) {
        // Create the head of the (*list)
        galaxy_object * new_object = NULL;
        new_object = calloc(1,sizeof(galaxy_object));
        DIE(!new_object, "Couldn't chain planets!\n");

        // Allocate the head
        new_object -> next = NULL;
        new_object -> prev = NULL;
        new_object -> data = NULL;
        new_object->data = malloc(sizeof(only_data_t));
        DIE(!((only_data_t*)(new_object->data)), "Couldn't chain planets!\n");

        // Save the data of the head
        ((only_data_t*)(new_object->data)) -> shields_number = shields_number;
        ((only_data_t*)(new_object->data)) -> destroyed_planets = 0;
        ((only_data_t*)(new_object->data)) -> name = NULL;
        ((only_data_t*)(new_object->data)) -> name = malloc(NAME_SIZE * sizeof(char));
        DIE(!((only_data_t*)(new_object->data)) -> name, "Couldn't chain planets!\n");
        strcpy(((only_data_t*)(new_object->data)) -> name, planet_name);

        // Create the shield (*list)
        galaxy_t * shield = NULL;
        shield = dll_create();
        DIE(!shield, "Couldn't create planet's shields!");
        int tmp = 1;
        for (int i = 0; i < shields_number; i++)
            dll_add_nth_node_shield(shield, 0, & tmp);
        ((only_data_t*)(new_object->data)) -> shield = shield;

        // Put the new node in the (*list)
        new_object -> prev = ( * list) -> head -> prev;
        ( * list) -> head -> prev -> next = new_object;
        new_object -> next = ( * list) -> head;
        ( * list) -> head -> prev = new_object;
        ( * list) -> galaxy_size++;
        printf("The planet %s has joined the galaxy.\n", (((only_data_t*)(new_object->data)) -> name));
        *global_size = *global_size + 1;

    } else { // Remove it from somewhere between the nodes
        // Create the head of the list
        galaxy_object * new_object = NULL;
        new_object = malloc(sizeof(galaxy_object));
        DIE(!new_object, "Couldn't chain planets!\n");

        // Allocate the head
        new_object -> next = NULL;
        new_object -> prev = NULL;
        new_object -> data = NULL;
        new_object->data = malloc(sizeof(only_data_t));
        DIE(!((only_data_t*)(new_object->data)), "Couldn't chain planets!\n");

        // Save the data of the head
        ((only_data_t*)(new_object->data)) -> shields_number = shields_number;
        ((only_data_t*)(new_object->data)) -> destroyed_planets = 0;
        ((only_data_t*)(new_object->data)) -> name = NULL;
        ((only_data_t*)(new_object->data)) -> name = malloc(NAME_SIZE * sizeof(char));
        DIE(!((only_data_t*)(new_object->data)) -> name, "Couldn't chain planets!\n");
        strcpy(((only_data_t*)(new_object->data)) -> name, planet_name);

        // Create the shield list
        galaxy_t * shield = NULL;
        shield = dll_create();
        DIE(!shield, "Couldn't create planet's shields!");
        int tmp = 1;
        for (int i = 0; i < shields_number; i++)
            dll_add_nth_node_shield(shield, 0, & tmp);
        ((only_data_t*)(new_object->data)) -> shield = shield;

        // Put the new node in the (*list)
        galaxy_object * start = ( * list) -> head;
        int index = planet_index % ( * list) -> galaxy_size;
        for (int i = 0; i < index - 1; i++)
            start = start -> next;

        new_object -> prev = start;
        new_object -> next = start -> next;
        start -> next -> prev = new_object;
        start -> next = new_object;
        ( * list) -> galaxy_size++;
        printf("The planet %s has joined the galaxy.\n", (((only_data_t*)(new_object->data))-> name));
        *global_size = *global_size + 1;
    }
}

// Remove the nth node
void dll_remove_nth_node_shield(galaxy_t ** list, unsigned int n)
{
    // Check the existence of the list
    if (! * list) {
        printf("No shield to be thrown in the void!\n");
        return;
    } else if (n < 0) {
        printf("Shield out of bounds!\n");
        return;
    } else if (!( * list) -> head) {
        printf("No shield to be thrown in the void!\n");
        free( * list);
        * list = NULL;
        ( * list) -> head = NULL;
        ( * list) -> galaxy_size = 0;
        return;
    } else if (!( * list) -> head -> next) { // If we have only one item
        ( * list) -> galaxy_size = 0;
        free(( * list) -> head -> data);
        ( * list) -> head -> next = NULL;
        ( * list) -> head -> prev = NULL;
        free(( * list) -> head);
        ( * list) -> head = NULL;
        free(( * list));
        ( * list) = NULL;
        return;
    } else if (n == 0) { // If the index is 0
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        ( * list) -> head -> prev -> next = ( * list) -> head -> next;
        ( * list) -> head -> next -> prev = ( * list) -> head -> prev;
        ( * list) -> head = start -> next;
        start -> next = NULL;
        start -> prev = NULL;
        free(start -> data);
        free(start);
        start = NULL;
        ( * list) -> galaxy_size--;
        return;
    } else if (n >= ( * list) -> galaxy_size - 1) { // If the index is bigger
        // than the list's size
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        while (start -> next -> next != ( * list) -> head) {
            start = start -> next;
        }
        galaxy_object * del = NULL;
        del = start -> next;
        start -> next = start -> next -> next;
        ( * list) -> head -> prev = start;
        free(del -> data);
        del -> next = NULL;
        del -> prev = NULL;
        ( * list) -> galaxy_size--;
        free(del);
        del = NULL;
        return;
    } else if (n > 0 && n < ( * list) -> galaxy_size - 1) {
        // if the element is between the nodes
        galaxy_object * start = ( * list) -> head;
        for (unsigned int i = 0; i < n; i++)
            start = start -> next;

        DIE(!start, "Couldn't remove node!\n");

        ( * list) -> galaxy_size--;
        start -> prev -> next = start -> next;
        start -> next -> prev = start -> prev;
        start -> next = NULL;
        start -> prev = NULL;

        free(start -> data);
        start -> data = NULL;
        free(start);
        start = NULL;
        return;
    }
}

// Remove a node from the nth position
void dll_remove_nth_node_p(galaxy_t ** list, unsigned int n) {
    // Remove a planet
    if (! * list) {
        printf("Planet out of bounds!\n");
        return;
    } else if (n < 0) {
        printf("Planet out of bounds!\n");
        return;
    } else if (!( * list) -> head) {
        printf("Planet out of bounds!\n");
        free( * list);
        * list = NULL;
        ( * list) -> head = NULL;
        ( * list) -> galaxy_size = 0;
        return;
    } else if (!( * list) -> head -> next) {
        // If we have only one item
        ( * list) -> galaxy_size = 0;
        free(( * list) -> head -> data);
        ( * list) -> head -> next = NULL;
        ( * list) -> head -> prev = NULL;
        free(( * list) -> head);
        ( * list) -> head = NULL;
        free(( * list));
        ( * list) = NULL;
        return;
    } else if (n == 0) {
        // If we want to delete the first item
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        ( * list) -> head -> prev -> next = ( * list) -> head -> next;
        ( * list) -> head -> next -> prev = ( * list) -> head -> prev;
        ( * list) -> head = start -> next;
        start -> next = NULL;
        start -> prev = NULL;
        free(start -> data);
        free(start);
        start = NULL;
        ( * list) -> galaxy_size--;
        return;
    } else if (n >= ( * list) -> galaxy_size - 1) {
        // If we want to delete an element bigger than the list's size
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        while (start -> next -> next != ( * list) -> head) {
            start = start -> next;
        }
        galaxy_object * del = NULL;
        del = start -> next;
        start -> next = start -> next -> next;
        ( * list) -> head -> prev = start;
        free(del -> data);
        del -> next = NULL;
        del -> prev = NULL;
        ( * list) -> galaxy_size--;
        free(del);
        del = NULL;
        return;
    } else if (n > 0 && n < ( * list) -> galaxy_size - 1) {
        // delete an item between the current nodes
        galaxy_object * start = NULL;
        start = dll_get_nth_node( * list, n);

        DIE(!start, "Couldn't remove node!\n");
        start = start -> next;
        ( * list) -> galaxy_size--;
        start -> prev -> next = start -> next;
        start -> next -> prev = start -> prev;
        start -> next = NULL;
        start -> prev = NULL;
        free(start -> data);
        start -> data = NULL;
        free(start);
        start = NULL;
        return;
    }
}

// Remove the nth node
void dll_remove_nth_node(galaxy_t ** list, unsigned int n, int* global_size) {
    // Remove nth node
    if (!*list) {
        printf("Planet out of bounds!\n");
        return;
    } else if (n < 0 || n >= (*list)->galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    } else if (!( * list) -> head) {
        printf("Planet out of bounds!\n");
        free( * list);
        * list = NULL;
        ( * list) -> head = NULL;
        ( * list) -> galaxy_size = 0;
        return;
    } else if (!( * list) -> head -> next) {// If we have only one item
        printf("The planet %s has been eaten by the vortex.\n", ((only_data_t * )( * list) -> head -> data) -> name);
        *global_size = *global_size - 1;
        ( * list) -> galaxy_size = 0;
        free(( * list) -> head -> data);
        ( * list) -> head -> next = NULL;
        ( * list) -> head -> prev = NULL;
        free(( * list) -> head);
        ( * list) -> head = NULL;
        free(( * list));
        ( * list) = NULL;
        return;
    } else if (n == 0) { // If we want to delete the first item
        printf("The planet %s has been eaten by the vortex.\n", ((only_data_t * )( * list) -> head -> data) -> name);
        *global_size = *global_size - 1;
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        ( * list) -> head -> prev -> next = ( * list) -> head -> next;
        ( * list) -> head -> next -> prev = ( * list) -> head -> prev;
        ( * list) -> head = start -> next;
        start -> next = NULL;
        start -> prev = NULL;
        free(start -> data);
        free(start);
        start = NULL;
        ( * list) -> galaxy_size--;
        return;
    } else if (n >= ( * list) -> galaxy_size-1) { // If we want to delete
        // from an index bigger than list's size
        *global_size = *global_size - 1;
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        while (start -> next -> next != ( * list) -> head) {
            start = start -> next;
        }
        galaxy_object * del = NULL;
        del = start -> next;
        start -> next = start -> next -> next;
        ( * list) -> head -> prev = start;
        printf("The planet %s has been eaten by the vortex.\n", ((only_data_t * )(del -> data)) -> name);
        free(del -> data);
        del -> next = NULL;
        del -> prev = NULL;
        ( * list) -> galaxy_size--;
        free(del);
        del = NULL;
        return;
    } else if (n > 0 && n < ( * list) -> galaxy_size - 1) {
        // delete a node from the current nodes
        *global_size = *global_size - 1;
        galaxy_object * start = NULL;
        start = dll_get_nth_node( * list, n);

        DIE(!start, "Couldn't remove node!\n");
        start = start -> next;
        ( * list) -> galaxy_size--;
        start -> prev -> next = start -> next;
        start -> next -> prev = start -> prev;
        start -> next = NULL;
        start -> prev = NULL;
        printf("The planet %s has been eaten by the vortex.\n", ((only_data_t * )(start -> data)) -> name);
        free(start -> data);
        start -> data = NULL;
        free(start);
        start = NULL;
        return;
    }
}

// Remove the nth node
// The same remove but with the message "implode"
void dll_remove_nth_node_implode(galaxy_t ** list, unsigned int n, int* global_size) {
    if (! * list) {
        printf("Planet out of bounds!\n");
        return;
    } else if (n < 0) {
        printf("Planet out of bounds!\n");
        return;
    } else if (!( * list) -> head) { // If we have no items
        printf("Planet out of bounds!\n");
        free( * list);
        * list = NULL;
        ( * list) -> head = NULL;
        ( * list) -> galaxy_size = 0;
        return;
    } else if (!( * list) -> head -> next) { // If we have only one item
        printf("The planet %s has imploded.\n", ((only_data_t * )( * list) -> head -> data) -> name);
          *global_size = *global_size - 1;
        ( * list) -> galaxy_size = 0;
        free(( * list) -> head -> data);
        ( * list) -> head -> next = NULL;
        ( * list) -> head -> prev = NULL;
        free(( * list) -> head);
        ( * list) -> head = NULL;
        free(( * list));
        ( * list) = NULL;
        return;
    } else if (n == 0) { // If we want to delete the first item
        printf("The planet %s has imploded.\n", ((only_data_t * )( * list) -> head -> data) -> name);
          *global_size = *global_size - 1;
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        ( * list) -> head -> prev -> next = ( * list) -> head -> next;
        ( * list) -> head -> next -> prev = ( * list) -> head -> prev;
        ( * list) -> head = start -> next;
        start -> next = NULL;
        start -> prev = NULL;
        free(start -> data);
        free(start);
        start = NULL;
        ( * list) -> galaxy_size--;
        return;
    } else if (n >= ( * list) -> galaxy_size - 1) { // Delete an item at a
        // bigger index than the list's size
          *global_size = *global_size - 1;
        if (( * list) -> galaxy_size == 1) {
            free(( * list) -> head -> data);
            ( * list) -> head -> next = NULL;
            ( * list) -> head -> prev = NULL;
            free(( * list) -> head);
            ( * list) -> head = NULL;
            ( * list) -> galaxy_size--;
            free( * list);
            * list = NULL;
            return;
        }
        galaxy_object * start = NULL;
        start = ( * list) -> head;
        while (start -> next -> next != ( * list) -> head) {
            start = start -> next;
        }
        galaxy_object * del = NULL;
        del = start -> next;
        start -> next = start -> next -> next;
        ( * list) -> head -> prev = start;
        printf("The planet %s has imploded.\n", ((only_data_t * )(del -> data)) -> name);
        free(del -> data);
        del -> next = NULL;
        del -> prev = NULL;
        ( * list) -> galaxy_size--;
        free(del);
        del = NULL;
        return;
    } else if (n > 0 && n < ( * list) -> galaxy_size - 1) {
        // delete a node from the current nodes
          *global_size = *global_size - 1;
        galaxy_object * start = NULL;
        start = dll_get_nth_node( * list, n);

        DIE(!start, "Couldn't remove node!\n");
        start = start -> next;
        ( * list) -> galaxy_size--;
        start -> prev -> next = start -> next;
        start -> next -> prev = start -> prev;
        start -> next = NULL;
        start -> prev = NULL;
        printf("The planet %s has imploded.\n", ((only_data_t * )(start -> data)) -> name);
        free(start -> data);
        start -> data = NULL;
        free(start);
        start = NULL;
        return;
    }
}

// Return the size of the list
unsigned int dll_get_size(galaxy_t * list) {
    return list -> galaxy_size;
}

// Release the memory
void dll_free(galaxy_t ** pp_list) {
    if (! * pp_list) {
        printf("List is already deleted!\n");
    } else if (!( * pp_list) -> head) {
        free( * pp_list);
        * pp_list = NULL;
    } else if (( * pp_list) -> head -> next == ( * pp_list) -> head) {
        // only one element
        free(( * pp_list) -> head -> data);
        ( * pp_list) -> head -> data = NULL;
        ( * pp_list) -> head -> next = NULL;
        ( * pp_list) -> head -> prev = NULL;
        free(( * pp_list) -> head);
        ( * pp_list) -> head = NULL;
        free( * pp_list);
    } else {
        galaxy_object * start = NULL;
        start = ( * pp_list) -> head;
        DIE(!start, "Couldn't release memory! It is already released!\n");
        galaxy_object * prev = NULL;
        do {
            prev = start;
            start = start -> prev;
            free(prev -> data);
            prev -> data = NULL;
            prev -> next = NULL;
            prev -> prev = NULL;
            free(prev);
            prev = NULL;
        }
        while (start != ( * pp_list) -> head);
        ( * pp_list) -> head = NULL;
        free(( * pp_list));
        ( * pp_list) = NULL;
    }
}

// Print int list
void dll_print_int_list(galaxy_t * list) {
    if (!list) {
        printf("No shields!\n");
        return;
    } else if (!list -> head) {
        printf("No shields\n");
        return;
    }

    galaxy_object * start = NULL;
    start = list -> head;
    do {
        printf("%d ", ( * (int * ) start -> data));
        start = start -> next;
    }
    while (start != list -> head);
}

// Print string list
void dll_print_string_list(galaxy_t * list) {
    if (!list) {
        printf("No planets!\n");
        return;
    } else if (!list -> head) {
        printf("No planets!\n");
        return;
    }

    galaxy_object * start = NULL;
    start = list -> head;
    do {
        printf("%s ", (char * ) start -> data);
        start = start -> next;
    }
    while (start != list -> head);

    printf("\n");
}
