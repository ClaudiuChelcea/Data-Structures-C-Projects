#include <stdio.h>
#include <stdlib.h>
#include "Shields.h"
#include <string.h>
#define GARBAGE - 232343

// Initialize the list
shield_List * init_head(int data_size) {
    shield_List * list = NULL;
    list = malloc(sizeof(shield_List));
    if (!list) {
        fprintf(stderr, "Error allocating head! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    list -> data_size = data_size;
    list -> size = 0;
    list -> head = NULL;

    return list;
}

// Add first element
void add_First(shield_List * list, void * new_data)
{
    if (!list) {
        list = init_head(list -> data_size);
        shield_t * new_node = NULL;
        new_node = calloc(1, sizeof(shield_t));

        if (!new_node) {
            fprintf(stderr, "Couldn't create list's first element!\n");
            exit(EXIT_FAILURE);
        }
        new_node -> data = NULL;
        new_node -> prev = NULL;
        new_node -> next = NULL;

        new_node -> data = calloc(1, list -> data_size);
        if (!new_node -> data) {
            fprintf(stderr, "Couldn't create list's first element!\n");
            exit(EXIT_FAILURE);
        }
        *((int * ) new_node -> data) = * ((int * ) new_data);
        list -> head = new_node;
        if (!list -> head) {
            fprintf(stderr, "Error creating list's head!\n");
            exit(EXIT_FAILURE);
        }
        list -> size++;

    } else if (!list -> head) {
        shield_t * new_node = NULL;
        new_node = calloc(1, sizeof(shield_t));
        if (!new_node) {
            fprintf(stderr, "Couldn't create list's first element!\n");
            exit(EXIT_FAILURE);
        }
        new_node -> data = NULL;
        new_node -> prev = NULL;
        new_node -> next = NULL;
        new_node -> data = calloc(1, list -> data_size);
        if (!new_node -> data) {
            fprintf(stderr, "Couldn't create list's first element!\n");
            exit(EXIT_FAILURE);
        }
        strncpy(new_node -> data, new_data, list -> data_size);
        list -> head = new_node;
        if (!list -> head) {
            fprintf(stderr, "Error creating list's head!\n");
            exit(EXIT_FAILURE);
        }
        list -> size++;
    } else {
        shield_t * new_node = NULL;
        new_node = calloc(1, sizeof(shield_t));
        if (!new_node) {
            fprintf(stderr, "Couldn't create list's first element!\n");
            exit(EXIT_FAILURE);
        }
        new_node -> data = NULL;
        new_node -> prev = NULL;
        new_node -> next = NULL;
        new_node -> data = calloc(1, list -> data_size);
        if (!new_node -> data) {
            fprintf(stderr, "Couldn't create list's first element!\n");
            exit(EXIT_FAILURE);
        }
        strncpy(new_node -> data, new_data, list -> data_size);

        new_node -> next = list -> head;
        list -> head -> prev = new_node;
        list -> head = new_node;
        list -> size++;
    }
}

// Display the list
void printList(shield_List * list) {
    if (!list || !list -> head) {
        fprintf(stderr, "No list to display!\n");
        return;
    }

    shield_t * new_node = NULL;
    new_node = list -> head;
    if (!new_node) {
        fprintf(stderr, "Couldn't display list!\n");
        return;
    }
    shield_t * new = NULL;
    while (new_node) {
        if (list -> data_size == sizeof(int)) {
            if (new_node -> next)
                printf("%d <--> ", *((int * ) new_node -> data));
            else {
                printf("%d", *((int * ) new_node -> data));
                new = new_node -> prev;
            }
            new_node = new_node -> next;
        } else if (list -> data_size == STRING_SIZE)
            printf("%s", (char * ) new_node -> data);

    }

    printf("\n\n");
}

// Release the memory
void releaseMemory(shield_List ** list) {

    if (! * list) {
        fprintf(stderr, "No list to release!\n");
        return;
    } else if (!( * list) -> head) {
        free( * list);
        * list = NULL;
    } else {
        shield_t * my_node = NULL;
        my_node = ( *list) -> head;
        if (!my_node) {
            fprintf(stderr, "Couldn't find head\n");
            exit(EXIT_FAILURE);
        }

        shield_t * del = NULL;
        while (my_node) {
            del = my_node;
            my_node = my_node -> next;
            free(del -> data);
			free(del);
            del -> next = NULL;
			del = NULL;
        }
        free( *list);
    }
}

// Remove duplicates from a sorted list
void removeDuplicates(shield_List * list) {
    if (!list) {
        fprintf(stderr, "No list to remove duplicates from!\n");
        return;
    }

    if (!list -> head -> next)
        return;

    shield_t * my_node = NULL;
    my_node = list -> head;
    if (!my_node) {
        fprintf(stderr, "Couldn't find head\n");
        exit(EXIT_FAILURE);
    }

    if (!my_node -> next)
        return;

    while (my_node -> next) {
        if ( *((int * ) my_node -> data) == *((int * ) my_node -> next -> data)) {
            if (my_node -> next -> next) {
                shield_t * node_del = my_node -> next;
                my_node -> next = my_node -> next -> next;
                my_node -> next -> prev = my_node;
                node_del -> prev = NULL;
                node_del -> next = NULL;
                free(node_del -> data);
                free(node_del);

            } else {
                free(my_node -> next -> data);
                my_node -> next -> prev = NULL;
                free(my_node -> next);
                my_node -> next = NULL;
            }
        } else
            my_node = my_node -> next;
    }
}

// Reverse the list
void reverseList(shield_List * list) {
    if (!list) {
        fprintf(stderr, "Couldn't reverse list!");
        exit(EXIT_FAILURE);
    }
    if (!list -> head) {
        fprintf(stderr, "List is empty!");
        exit(EXIT_FAILURE);
    }
    if (!list -> head -> next)
        return;

    shield_t * start = NULL;
    start = list -> head;
    if (!start) {
        fprintf(stderr, "Couldn't reverse list!");
        exit(EXIT_FAILURE);
    }
    shield_t * tmp = NULL;

    while (start) {
        tmp = start -> prev;
        start -> prev = start -> next;
        start -> next = tmp;
        start = start -> prev;
    }
    list -> head = tmp -> prev;

}

// Sum of two lists
shield_List * sum_list_funct(shield_List * A, shield_List * B) {
    if (!A || !B || !A -> head || !B -> head) {
        fprintf(stderr, "Couldn't execute sum! One list is missing!\n");
        exit(EXIT_FAILURE);
    }

    shield_t * startA = A -> head;
    shield_t * startB = B -> head;
    shield_List * new_list = NULL;
    new_list = init_head(sizeof(int));
    if (A -> size > B -> size) {
        while (startB) {
            int sum = * ((int * ) startA -> data) + * ((int * ) startB -> data);
            add_First(new_list, & sum);
            startA = startA -> next;
            startB = startB -> next;
        }
        while (startA) {
            int sum = * ((int * ) startA -> data);
            add_First(new_list, & sum);
            startA = startA -> next;
        }
    } else if (A -> size < B -> size) {
        while (startA) {
            int sum = * ((int * ) startA -> data) + * ((int * ) startB -> data);
            add_First(new_list, & sum);
            startA = startA -> next;
            startB = startB -> next;
        }
        while (startB) {
            int sum = * ((int * ) startB -> data);
            add_First(new_list, & sum);
            startB = startB -> next;
        }
    } else {
        while (startA) {
            int sum = * ((int * ) startA -> data) + * ((int * ) startB -> data);
            add_First(new_list, & sum);
            startA = startA -> next;
            startB = startB -> next;
        }
    }
    return new_list;
}

// Swap to integers
void swapIntegers(int * a, int * b) {
    int tmp = * a;
    * a = * b;
    * b = tmp;
}

// Sort a list by bubble sort
void sortList(shield_List ** list) {
    if (! * list || !( * list) -> head) {
        fprintf(stderr, "No list to sort!\n");
        return;
    }
    if (!( * list) -> head -> next)
        return;

    for (shield_t * my_i = ( * list) -> head; my_i != NULL; my_i = my_i -> next) {
        for (shield_t * my_j = my_i -> next; my_j != NULL; my_j = my_j -> next) {
            if ( * ((int * ) my_i -> data) > * ((int * ) my_j -> data)) {
                swapIntegers(((int * ) my_i -> data), ((int * ) my_j -> data));
            }
        }
    }
}

// Merge two sorted lists
shield_List * merge_lists_funct(shield_List * A, shield_List * B) {
    if (!A || !B || !A -> head || !B -> head) {
        fprintf(stderr, "Couldn't execute merge! One list is missing!\n");
        exit(EXIT_FAILURE);
    }

    shield_t * startA = A -> head;
    shield_t * startB = B -> head;

    shield_List * new_list = NULL;
    new_list = init_head(sizeof(int));
    if (!new_list) {
        fprintf(stderr, "Couldn't merge lists!\n");
        exit(EXIT_FAILURE);
    }

    while (startA) {
        add_First(new_list, ((int * ) startA -> data));
        startA = startA -> next;
    }
    while (startB) {
        add_First(new_list, ((int * ) startB -> data));
        startB = startB -> next;
    }
    sortList( & new_list);

    return new_list;
}

// T-H delete for the element in the middle of the list
void Tortoise_and_Hare_delete(shield_List ** list) {
    // No elements
    if (! * list) {
        fprintf(stderr, "No list to delete from!\n");
        exit(EXIT_FAILURE);
    }

    // One element
    else if (!( * list) -> head -> next) {
        free(( * list) -> head -> data);
        free(( * list) -> head);
        ( * list) -> head = NULL;
        free( * list);
        * list = NULL;
        return;
    }

    // Two elements
    else if (( * list) -> head -> next -> next == NULL) {
        ( * list) -> head = ( * list) -> head -> next;
        free(( * list) -> head -> prev -> data);
        ( * list) -> head -> prev -> next = NULL;
    }
    // More elements
    else {
        shield_t * tortoise = NULL;
        tortoise = ( * list) -> head;
        shield_t * hare = NULL;
        hare = tortoise;

        if (!tortoise || !hare) {
            fprintf(stderr, "Couldn't delete middle element\n");
            exit(EXIT_FAILURE);

        }

        while (hare -> next && hare -> next -> next && tortoise -> next) {

            hare = hare -> next -> next;
            tortoise = tortoise -> next;
        }

        shield_t * skip = tortoise -> prev;
        if (!skip) {
            fprintf(stderr, "Couldn't delete middle element\n");
            exit(EXIT_FAILURE);
        }
        skip -> next = skip -> next -> next;
        skip -> next -> prev = skip;
        tortoise -> prev = NULL;
        tortoise -> next = NULL;
        free(tortoise -> data);
        free(tortoise);
    }
}
