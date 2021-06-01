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

RopeNode* node_concat(RopeNode* rt1, RopeNode* rt2)
{
    // Create new root
    RopeNode* new_root = makeRopeNode(strdup(EMPTY));

    // Assing new values
    new_root->right = rt2;
    new_root->left  = rt1;
    new_root->weight = getTotalWeight(rt1);

    // Return the newly created root
    return new_root;
}

// The same concat,
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

// Split a node in two parts
void splitNode(RopeNode* node, int index) {
    char* str1 = calloc(100,sizeof(char));
    for(int i=0;i<index;i++)
        str1[i] = node->str[i];
    
    char* str2 = calloc(100,sizeof(char));
    for(size_t i=index;i<strlen(node->str);i++)
        str2[i-index] = node->str[i];

    node->left = makeRopeNode(str1);
    node->right = makeRopeNode(str2);
}

// Inorder traversal
void inorder(RopeNode* node, RopeNode** orphanans, int index, int *write, int* found) {
    if(!node)
        return;

    if(!node->left && !node->right) {
        if(index == node->weight) {
            *found = 1;
            return;
        }
        else {
            splitNode(node,index);
            index = node->left->weight;
        }
    }

    // Inorder traversal until we find node
    if(!(*found)) {
        // Go left if the weight is bigger than our index
        if(index <=node->weight) {
            inorder(node->left, orphanans, index, write,found);
        }
        // Otherwise go right
        else {
            inorder(node->right,orphanans,index - node->weight, write, found);
        }
    }
    if(index <= node->weight)  {
        orphanans[(*write)++] = node->right;
        node->right = NULL;
    }
}

// Release nodes
void __free_Nodes(RopeNode** root) {
    if(!(*root))
        return;
        
    free((void*)(*root)->str);
    __free_Nodes(&(*root)->left);
    __free_Nodes(&(*root)->right);
    free((*root));
}

// Release tree
void free_Tree(RopeTree* rt) {
    __free_Nodes(&(rt->root));

}
// Split NODE
SplitPair split(RopeTree* rt, int idx)
{
    // Create tree duplicate to not change the original tree
    RopeTree* copy = copyRopeTree(rt);

    // Create subtrees to be returned
    SplitPair sp_trees;
    sp_trees.left = NULL;
    sp_trees.right = NULL;
    sp_trees.left = copy->root;

    // If the index is '0'
    if(idx == 0) {
        // Th
        sp_trees.left = copy->root;
        // sp_trees.left = makeRopeNode(strdup(EMPTY));
        // sp_trees.right = copy->root;
        free(copy);
        return sp_trees;
    }
    
    // If we want to perfectly split
    if(idx == getTotalWeight(copy->root)) {
         sp_trees.right =makeRopeNode(strdup(EMPTY));
         free(copy);
         return sp_trees;
         
    }
    
    // Split the tree in half
    if(idx == rt->root->weight) {
        sp_trees.right = sp_trees.left->right;
        sp_trees.left->right = NULL;
        free(copy);
        return sp_trees;
    }

    // Get nodes to be appened to the right subtree
    RopeNode** orphans = calloc(100,sizeof(RopeNode*));
    int write = 0, found  = 0;
    inorder(sp_trees.left,orphans,idx,&write,&found);


    if(write > 1){
        sp_trees.right = node_concat(orphans[0], orphans[1]);
        for(int i=2;i<write;++i) {
            sp_trees.right = node_concat(sp_trees.right,orphans[i]);
        }
    }
    else {
            sp_trees.right = node_concat(sp_trees.right,orphans[0]);
    }
    free(orphans);
    free(copy);    
    return sp_trees;
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

    for (int i = 0; i < 15; ++i) {
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