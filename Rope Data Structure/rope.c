/* Copyright Chelcea Claudiu-Marian & Brinzan  Darius-Ionut */
#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""

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


int getTotalWeight(RopeNode* rt) {
    if (!rt)
        return 0;

    return rt->weight + getTotalWeight(rt->right);
}


RopeTree* concat(RopeTree* rt1, RopeTree* rt2)
{
    // Create new tree and assign it an empty root
    RopeTree* new_Tree = makeRopeTree(makeRopeNode(strdup(EMPTY)));

    // Assing new values
    new_Tree->root->right = rt2->root;
    new_Tree->root->left  = rt1->root;
    new_Tree->root->weight = getTotalWeight(rt1->root);

    // Return the newly created tree
    return new_Tree;
}

char __indexRope(RopeNode* root, int idx)
{
    // Search by index
    if(!root->left && !root->right) {
        // Return character
        return root->str[idx];
    }

    // Get through all the nodes
    if(root->weight <= idx)
        return __indexRope(root->right, idx-root->weight);
    else
        return __indexRope(root->left, idx);
}

char indexRope(RopeTree* rt, int idx)
{
    return __indexRope(rt->root, idx);
}

char* search(RopeTree* rt, int start, int end) {
    // TODO 3. Search - 20p
     return NULL;
}

SplitPair split(RopeTree* rt, int idx) {
    // TODO 4. Split - 20p
    SplitPair my_pair;
     return my_pair;
}

RopeTree* insert(RopeTree* rt, int idx, const char* str) {
    // TODO 5. Insert - 5p
     return NULL;
}

RopeTree* delete(RopeTree* rt, int start, int len) {
    // TODO 6. Delete - 5p
    return NULL;
}

// FINAL 10p -> complex test involving all operations

int main() {

    RopeNode *rn1, *rn2, *rn3, *rn4, *rn5, *rn6, *rn7;
    RopeTree *rt;

    rn1 = makeRopeNode("abc");
    rn2 = makeRopeNode("de");
    rn3 = makeRopeNode("fghi");
    rn4 = makeRopeNode("jklmn");
    rn5 = makeRopeNode(EMPTY);
    rn6 = makeRopeNode(EMPTY);
    rn7 = makeRopeNode(EMPTY);

    rn5->left = rn1;
    rn5->right = rn2;
    rn5->weight = 3;

    rn6->left = rn3;
    rn6->right = rn4;
    rn6->weight = 4;

    rn7->left = rn5;
    rn7->right = rn6;
    rn7->weight = 5;

    rt = makeRopeTree(rn7);

    for (int i = 0; i < 14; ++i) {
        printf("Character at index %d is %c\n", i, indexRope(rt, i));
    }

    free(rn1);
    free(rn2);
    free(rn3);
    free(rn4);
    free(rn5);
    free(rn6);
    free(rn7);
    free(rt);

    return 0;
}