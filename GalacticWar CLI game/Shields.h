#ifndef __SHIELDS_H_
#define __SHIELDS_H_
#define STRING_SIZE 64

// Nodes
typedef struct shield {
    struct shield * next;
    struct shield * prev;
    void * data;
}
shield_t;

// The head of the list
typedef struct shield_List {
    unsigned int data_size;
    unsigned int size;
    shield_t * head;
}
shield_List;

// Initialize the list
shield_List *init_head(int data_size);

// Add first element
void add_First(shield_List * list, void * new_data);

// Display the list
void printList(shield_List * list);

// Release the memory
void releaseMemory(shield_List ** list);

// Remove duplicates from list
void removeDuplicates(shield_List * list);

// Reverse the list
void reverseList(shield_List * list);

// Sum of two lists
shield_List * sum_list_funct(shield_List * A, shield_List * B);

// Swap to integers
void swapIntegers(int * a, int * b);

// Sort a list by bubble sort
void sortList(shield_List ** list);

// Merge two sorted lists
shield_List * merge_lists_funct(shield_List * A, shield_List * B);

// T-H delete for the element in the middle of the list
void Tortoise_and_Hare_delete(shield_List ** list);

#endif
