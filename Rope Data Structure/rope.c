#include "./rope.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define EMPTY ""

typedef enum bool { false, true} bool;

char* strdup(const char*);

// Print the whole tree
void printRopeNode(RopeNode* rn) {
    if (!rn)
        return;

    if (!(rn->left) && !(rn->right)) {
        printf("%s", rn->str);
        return;
    }

    printRopeNode(rn->left);
    printRopeNode(rn->right);
}

// Print the whole tree
void printRopeTree(RopeTree* rt) {
    if (rt && rt->root) {
        printRopeNode(rt->root);
        printf("%s", "\n");
    }
}

// Debugging function
void debugRopeNode(RopeNode* rn, int indent) {
    if (!rn)
        return;

    for (int i = 0; i < indent; ++i)
        printf("%s", " ");

    if (!strcmp(rn->str, EMPTY))
        printf("# %d\n", rn->weight);
    else
        printf("%s %d\n", rn->str, rn->weight);

    debugRopeNode(rn->left, indent+2);
    debugRopeNode(rn->right, indent+2);
}

// Get the total weight of a node
int getTotalWeight(RopeNode* rt) {
    if (!rt)
        return 0;

    return rt->weight + getTotalWeight(rt->right);
}

// Create a node with the given text
RopeNode* makeRopeNode(const char* str)
{
    // Allocate memory for the node
    RopeNode* new_Node = NULL;
    new_Node = calloc(1, sizeof(RopeNode));
    DIE(!new_Node, "Couldn't create new node!");

    // Fill node's fields
    new_Node->left = NULL;
    new_Node->right = NULL;
    new_Node->str = str;
    new_Node->weight = strlen(new_Node->str);

    // Return the created node
    return new_Node;
}

// Create a tree having the variable root as the tree's root
RopeTree* makeRopeTree(RopeNode* root)
{
    // Create tree
    RopeTree* new_Tree = NULL;
    new_Tree = calloc(1, sizeof(RopeTree));
    DIE(!new_Tree, "Couldn't create tree!");

    // Assign tree's root
    new_Tree->root = root;

    // Return the created tree
    return new_Tree;
}

// Concatenate two nodes
RopeNode* node_concat(RopeNode* rt1, RopeNode* rt2)
{
    // Create new root
    RopeNode* new_root = makeRopeNode((char*) strdup(EMPTY));

    // Assing new values
    new_root->right = rt2;
    new_root->left  = rt1;
    new_root->weight = getTotalWeight(rt1);

    // Return the newly created root
    return new_root;
}

// Concatenate two trees
RopeTree* concat(RopeTree* rt1, RopeTree* rt2)
{
    // Create new tree and assign it an empty root
    RopeTree* new_Tree = NULL;
    new_Tree = makeRopeTree(makeRopeNode((char*) strdup(EMPTY)));

    // Assing new values
    new_Tree->root->right = rt2->root;
    new_Tree->root->left  = rt1->root;
    new_Tree->root->weight = getTotalWeight(rt1->root);

    // Return the newly created tree
    return new_Tree;
}

// Return the character at a certain position in the rope
char __indexRope(RopeNode* root, int index)
{
    if (!root) {
        return ' ';
    }

    // Search by index
    if (root && !root -> left && !root -> right) {
        // Return character if we are on a leaf
        return root->str[index];
    }

    // Search through the nodes recursively
    if (root->weight <= index) {
        return __indexRope(root->right, index-root->weight);
    } else {
        return __indexRope(root->left, index);
    }
}

// Return the character at the 'idx' position
char indexRope(RopeTree* rt, int idx)
{
    return __indexRope(rt->root, idx);
}

// Copy an entire tree
RopeNode* __copy_Rope_Node(RopeNode* rn)
{
    // If it doesn't exist, return NULL
    if (!rn)
        return NULL;

    // Create duplicate and ge it's weight
    RopeNode* new_rn = makeRopeNode((char*) strdup(rn->str));
    new_rn->weight = rn->weight;

    // Recursively copy the rest of the nodes
    new_rn->left = __copy_Rope_Node(rn->left);
    new_rn->right = __copy_Rope_Node(rn->right);

    // Return the tree(a node with links to another nodes)
    return new_rn;
}

// Copy a tree
RopeTree* __copy_Rope_Tree(RopeTree* rt)
{
    // If the tree is NULL, return NULL
    if (!rt)
        return NULL;

    // Else copy it
    return makeRopeTree(__copy_Rope_Node(rt->root));
}


// Search a string from 'start' to 'end' positions
char* search(RopeTree* rt, int start, int end)
{
    // Create string to be returned
    char* searched_string = NULL;
    searched_string = calloc(NODE_DATA_MAX_LENGTH, sizeof(char));
    DIE(!searched_string, "Couldn't search string!");

    // Save in the string all the break_condition items
    for (int i = start; i < end; i++) {
        searched_string[i - start] = indexRope(rt, i);
    }

    // Return the string's address
    return searched_string;
}

// Return the leaf on which we can find the element at position 'index'
RopeNode* return_wanted_leaf(RopeNode* root, int index)
{
    // If we can't go deeper, return the node
    if (!root->left && !root->right) {
        return root;
    }

    // Go down the tree recursively
    if (root->weight >= index) {
        return return_wanted_leaf(root->left, index);
    } else if (root->weight < index) {
        return return_wanted_leaf(root->right, index - root->weight);
    }
    return NULL;
}

// Allocate string and copy contents
char* new_string(int start, int stop, char* source)
{
    // Receive the part of the string
    char* my_string = NULL;
    my_string = calloc(NODE_DATA_MAX_LENGTH, sizeof(char));
    DIE(!my_string, "Couldn't split node!");

    // Copy content in the string
    int position = 0;
    for (int i = start; i< stop; i++)
        my_string[position++] = source[i];

    // Return the new string
    return my_string;
}

// Split a node in two parts
void splitNode(RopeNode* node, int index)
{
    // Receive the strings for the new nodes
    char* str1 = new_string(0, index, (void*) node->str);
    char* str2 = new_string(index, strlen(node->str), (void*) node->str);

    // Create a right node and a left node as childs of the current node
    node->right = makeRopeNode(str2);
    node->left = makeRopeNode(str1);
}

// Inorder traversal to store in the 'nodes_to_move' variable all nodes
// than are after index and therefore in the right subtree
void get_extra_nodes(RopeNode** nodes_to_move, RopeNode* root,
                     int *nr_elements, bool* break_condition, int index)
{
    // Break recursivity
    if (!root) {
        return;
    }

    // If we are on the leaf
    if (!root->left && !root->right) {
        // If we have broken down the leaf enough so that the index
        // matches the root's weight, set break_condition to 1
        // to start returning all other nodes from now on in nodes_to_move
        // variable
        if (index == root->weight) {
            *break_condition = 1;
            return;
        } else {  // Else split the node in two nodes Example: From abc we will
            splitNode(root, index);  // have node->left='a' && node->right ='bc'
            index = root->left->weight;  // while the curr. node stays the same
        }
    }

    // Inorder traversal until we find node at the 'index' position
    // only as long as the break_condition has not been fullfilled
    if (!(*break_condition)) {
        // Go left if the weight is bigger than our index
        if (index <= root->weight) {
            get_extra_nodes(nodes_to_move, root->left,
                            nr_elements, break_condition, index);
        } else {  // Otherwise go right
            get_extra_nodes(nodes_to_move, root->right, nr_elements,
                            break_condition, index - root->weight);
        }
    }

    // While going up the tree from the recursivity
    // Put all the right nodes that don't fit in the index
    // in the vector intended to move them to the right subtree
    if (root->weight >= index)  {
        nodes_to_move[*nr_elements] = root->right;  // Save element
        root->right = NULL;  // Break link from the left subtree
        ++(*nr_elements);   // Increase the number of elements
    }
}

// Release nodes in an postorder fashion
void __free_Nodes(RopeNode** root)
{
    // Stop recursivity
    if (!(*root)) {
        return;
    }
    // Recursive calls on all the nodes of the tree
    __free_Nodes(&(*root)->left);
    __free_Nodes(&(*root)->right);
    free((void*)(*root)->str);
    free((*root));
}

// Initialise pair
SplitPair __make_pair(RopeNode* new_root)
{
    // Create new pair and initialise it
    SplitPair new_pair;
    new_pair.left = new_root;
    new_pair.right = NULL;
    return new_pair;   // Return it
}

// Recalculate weights for the nodes
void rebalance_weights(RopeNode* root)
{
    // Inorder traversal to recalculate the weights
    // in the split function
    if (!root) {
        return;
    }

    // Go to the left
    rebalance_weights(root->left);

    // If it's a leaf, it's weight is equal to it's string length
    if (!root->left && !root->right) {
        root->weight = strlen(root->str);
    } else {  // Else the weight of the current node is equal to the left tree
        root->weight = getTotalWeight(root->left);
    }

    // Go to the right
    rebalance_weights(root->right);
}

// Split a tree and it's nodes at 'idx' position
SplitPair split(RopeTree* rt, int idx)
{
    // Create tree duplicate to not change the original tree
    RopeTree* new_Tree = __copy_Rope_Tree(rt);

    // Create subtrees to be returned
    SplitPair my_pair = __make_pair(new_Tree->root);

    // Special case 1: if the index is '0'
    if (idx == 0) {  // Return the already built tree
        my_pair.right = new_Tree->root;
        my_pair.left = makeRopeNode((char*) strdup(EMPTY));
        free(new_Tree);  // Release memory
        return my_pair;  // Return trees
    }

    // Special case 2: If everything is in the left tree
    if (getTotalWeight(new_Tree->root) == idx) {   // Set new trees
        my_pair.right = makeRopeNode((char*) strdup(EMPTY));
        my_pair.left = new_Tree->root;
        free(new_Tree);  // Release memory
        return my_pair;  // Return trees
    }

    //  Save the nodes that need to be moved in a vector for O(1) access time
    int nr_elements = 0;
    bool break_condition = false;
    RopeNode** nodes_to_move = calloc(MAX_NODES_TO_MOVE, sizeof(RopeNode*));

    // Add in the vector all extra elements(that are after 'idx')
    get_extra_nodes(nodes_to_move, my_pair.left, &nr_elements,
                    &break_condition, idx);

    // Move all elements from the vector to the right tree
    for (int i = 0; i < nr_elements; ++i) {
        my_pair.right = node_concat(my_pair.right, nodes_to_move[i]);
    }
    // Release memory
    free(nodes_to_move);
    free(new_Tree);

    // Recalculate weight for the left tree
    rebalance_weights(my_pair.left);
    return my_pair;  // Return the two newly formed trees
}

// Insert a string in our current rope data structure
RopeTree* insert(RopeTree* rt, int idx, const char* str)
{
    // Split the current string at the 'idx' position
    SplitPair pair = split(rt, idx);

    // Concatenate left tree with the new string and then with the right tree
    return makeRopeTree(node_concat(node_concat((pair.left),
                        makeRopeNode(str)), pair.right));
}

// Delete a string from the current string held in the rope data structure
RopeTree* delete(RopeTree* rt, int start, int len)
{
    SplitPair pair1 = split(rt, start);  // Split str at the 'start' position
    // Split the right string again
    SplitPair pair2 = split(makeRopeTree(pair1.right), len);

    // Return the remainder
    return makeRopeTree(node_concat(pair1.left, pair2.right));
}

const char* strs[] = {"123", "1234", "1234567", "0", "!?"};

int main() {

    RopeTree *r1, *r2, *r3, *r4, *r5, *r6;
    r1 = makeRopeTree(makeRopeNode("A"));
    r2 = makeRopeTree(makeRopeNode("BCDE"));
    r3 = makeRopeTree(makeRopeNode("F"));
    r4 = makeRopeTree(makeRopeNode("GH"));
    r5 = makeRopeTree(makeRopeNode("IJKL"));
    r6 = makeRopeTree(makeRopeNode("M"));

    RopeTree* rtc = concat(concat(concat(concat(concat(r1, r2), r3), r4), r5), r6);
    printRopeTree(rtc);

    rtc = insert(rtc, 12, "123");
    printRopeTree(rtc);
    rtc = insert(rtc, 3, "45");
    printRopeTree(rtc);
    rtc = insert(rtc, 3, "67");
    printRopeTree(rtc);

    rtc = delete(rtc, 0, 2);
    printRopeTree(rtc);
    rtc = delete(rtc, 12, 1);
    printRopeTree(rtc);
    rtc = delete(rtc, 5, 3);
    printRopeTree(rtc);

    printf("str[4]:%c\nstr[8]:%c\nstr[10]:%c\n", indexRope(rtc, 4), indexRope(rtc, 8), indexRope(rtc, 10));

    printf("str[0:2]:%s\nstr[4:7]:%s\nstr[9:10]:%s\n", search(rtc, 0, 2), search(rtc, 4, 7), search(rtc, 9, 10));

    return 0;
}
