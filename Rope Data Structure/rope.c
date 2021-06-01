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
    RopeTree* new_Tree = NULL;
    new_Tree = makeRopeTree(makeRopeNode(strdup(EMPTY)));

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
    if (!root -> left && !root -> right) {
        // Return character
        return root->str[idx];
    }

    // Get through all the nodes
    if (root->weight <= idx) {
        return __indexRope(root->right, idx-root->weight);
    } else {
        return __indexRope(root->left, idx);
    }
}

char indexRope(RopeTree* rt, int idx)
{
    return __indexRope(rt->root, idx);
}

char* search(RopeTree* rt, int start, int end)
{
    // Create string to be returned
    char* searched_string = NULL;
    searched_string = calloc(NODE_DATA_MAX_LENGTH, sizeof(char));
    DIE(!searched_string, "Couldn't search string!");

    // Save in the string all the found items
    for (int i = start; i < end; i++) {
        searched_string[i - start] = indexRope(rt, i);
    }

    // Return the string's address
    return searched_string;
}

RopeNode* return_wanted_leaf(RopeNode* root, int index)
{
    if(!root->left && !root->right) {
        return root;
    }

    if(root->weight >= index) {
        return return_wanted_leaf(root->left,index);
    } else if(root->weight < index) {
        return return_wanted_leaf(root->right,index - root->weight);
    }
    return NULL;
}

// split tree
void split_tree(RopeNode** root_l, RopeNode** root_r, int index, RopeTree** new_tree)
{
    if(!*root_l) {
        return;
    }

    split_tree(&((*root_l)->left),root_r, index, new_tree);
   
    // If the node is not a leaf
    if(strcmp((*root_l)->str,"") == 0) {
        
        // Get left and right weights
        int weight_left = 0;
        if((*root_l)->left)
            weight_left = (*root_l)->weight;
        int weight_right = 0;
        if((*root_l)->right)
            weight_right = (*root_l)->right->weight;

        // 
        if(weight_left + weight_right > index) 
        {
           
            // Move_to the right tree
            if(*root_r == NULL) {
                *root_r = (*root_l)->right;

                // 2---------
                (*root_l)->right = NULL;
                (*root_l)->weight = index;
            }
            else {
                *new_tree = concat(makeRopeTree(*root_r),makeRopeTree((*root_l)->right));
                (*root_r) = (*new_tree)->root;
                //  2-----------
                (*root_l)->right = NULL;
            }
        }
        else if(weight_left + weight_right == index) {
            // Move_right
            if(*root_r == NULL) {
                *root_r = (*root_l)->right->right;

                // 2---------
                (*root_l)->right->right = NULL;
                (*root_l)->weight = index;
            }
            else {
                *new_tree = concat(makeRopeTree(*root_r),makeRopeTree((*root_l)->right->right));
                (*root_r) = (*new_tree)->root;
                //  2-----------
                (*root_l)->right->right = NULL;
            }
        }  
    }

    split_tree(&((*root_l)->right),root_r,index, new_tree);

    // Return splitted tree
    return;
}

// Split NODE
SplitPair split(RopeTree* rt, int idx)
{
    RopeTree* new_tree = copyRopeTree(rt);

    // Create pair to be returned
    SplitPair my_pair;
    my_pair.left = NULL;
    my_pair.right = NULL;

    // If index is 0
    if(idx == 0) {
        my_pair.left = new_tree->root;
        return my_pair;
    } else {
        // First leaf
        RopeNode* check_first = new_tree->root;
        while(check_first->left) {
            check_first = check_first->left;
        }

        // Get down to leaf
        RopeNode* my_node = return_wanted_leaf(new_tree->root, idx);

        if(my_node == check_first) {
            // First leaf
            char* node1_text = calloc(100, sizeof(char));
            for(int i=0;i<idx;i++) {
                node1_text[i] = my_node->str[i];
            }
            char* node2_text = calloc(100, sizeof(char));
            for(int i=idx;i<my_node->weight;i++) {
                node2_text[i-idx] = my_node->str[i];
            }

            my_node->left = makeRopeNode(strdup(node1_text));
            my_node->right = makeRopeNode(strdup(node2_text));

            // 1----------
            my_node->str = strdup("");
            my_node->weight = my_node->left->weight;
            
            my_pair.left = new_tree->root;

        }
        else {
            char* node1_text = calloc(100, sizeof(char));
            for(int i=0;i<idx - 1 - my_node->weight;i++) {
                node1_text[i] = my_node->str[i];
            }
            char* node2_text = calloc(100, sizeof(char));
            int count = 0;
            for(int i=idx-1-my_node->weight;i<my_node->weight;i++) {
                node2_text[count++] = my_node->str[i];
            }

            my_node->left = makeRopeNode(strdup(node1_text));
            my_node->right = makeRopeNode(strdup(node2_text));
            // 1-----------
            my_node->str = strdup("");
            my_node->weight = my_node->left->weight;
            
            my_pair.left = new_tree->root;
        }
    }

    // Return output
    RopeTree* new_tree_2;
    split_tree(&(my_pair.left),&(my_pair.right), idx, &new_tree_2);
    return my_pair;
}

RopeTree* insert(RopeTree* rt, int idx, const char* str) {
    SplitPair pair = split(rt, idx);
    return concat(concat(makeRopeTree(pair.left),
                         makeRopeTree(makeRopeNode(str))), 
                  makeRopeTree(pair.right));
}

RopeTree* delete(RopeTree* rt, int start, int len) {
    SplitPair pair1 = split(rt, start);
    SplitPair pair2 = split(makeRopeTree(pair1.right), start + len);
    return concat(makeRopeTree(pair1.left),
                  makeRopeTree(pair2.right));
}

// FINAL 10p -> complex test involving all operations

char* strdup(const char*);

RopeNode* rnallocated[1000];
RopeTree* rtallocated[1000];
int numrn = 0;
int numrt = 0;

RopeNode * checkerMakeRopeNode(const char *str) {
   rnallocated[numrn] = makeRopeNode(str);
   return rnallocated[numrn++];
}

RopeTree * checkerMakeRopeTree(RopeNode * rn) {
   rtallocated[numrt] = makeRopeTree(rn);
   return rtallocated[numrt++];
}   

RopeNode* copyRopeNode(RopeNode* rn) {
    if (!rn)
        return NULL;

    RopeNode* new_rn = makeRopeNode(strdup(rn->str));
    new_rn->weight = rn->weight;
    new_rn->left = copyRopeNode(rn->left);
    new_rn->right = copyRopeNode(rn->right);
    return new_rn;
}

RopeTree* copyRopeTree(RopeTree* rt) {
    if (!rt)
        return NULL;

    RopeTree* new_rt = makeRopeTree(copyRopeNode(rt->root));
    return new_rt;
}

void freeRopeNode(RopeNode* rn) {
    if (rn == NULL)
        return;
    for (int i = 0; i < numrn; ++i)
        if (rn == rnallocated[i])
            return;

    free((void*)rn->str);
    freeRopeNode(rn->left);
    freeRopeNode(rn->right);
    free(rn);
}

//void freeRopeTree(RopeTree* rt) {
//    if (!rt)
//        return;
//
//    freeRopeNode(rt->root);
//}

int main() {

    RopeNode *rn1, *rn2, *rn3, *rn4, *rn5, *rn6, *rn7;
    RopeTree *rt, *rt1, *rt2;

    rn1 = checkerMakeRopeNode(strdup("abc"));
    rn2 = checkerMakeRopeNode(strdup("de"));
    rn3 = checkerMakeRopeNode(strdup("fghi"));
    rn4 = checkerMakeRopeNode(strdup("jklmn"));
    rn5 = checkerMakeRopeNode(strdup(EMPTY));
    rn6 = checkerMakeRopeNode(strdup(EMPTY));
    rn7 = checkerMakeRopeNode(strdup(EMPTY));

    rn5->left = rn1;
    rn5->right = rn2;
    rn5->weight = 3;

    rn6->left = rn3;
    rn6->right = rn4;
    rn6->weight = 4;

    rn7->left = rn5;
    rn7->right = rn6;
    rn7->weight = 5;

    rt = checkerMakeRopeTree(rn7);

    for (int i = 0; i < 7; ++i) {
        SplitPair sp = split(rt, i); 
        rt1 = makeRopeTree(sp.left);
        printRopeTree(rt1);
        rt2 = makeRopeTree(sp.right);
        printRopeTree(rt2);
        printf("\n");
        freeRopeNode(sp.left);
        free(rt1);
        freeRopeNode(sp.right);
        free(rt2);
    }

    for (int i = 0; i < numrn; i++) {
        free((void*)rnallocated[i]->str);
        free(rnallocated[i]);
    }

    for (int i = 0; i < numrt; i++) {
        free(rtallocated[i]);
    }

    return 0;
}