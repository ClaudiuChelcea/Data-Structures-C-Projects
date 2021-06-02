/* Copyright Chelcea Claudiu-Marian & Brinzan Darius-Ionut */
#ifndef __ROPE__
#define __ROPE__
#define EMPTY ""
#define NODE_DATA_MAX_LENGTH 20
#define MAX_NODES_TO_MOVE 50

typedef struct RopeNode RopeNode;

struct RopeNode {
    RopeNode* left;
    RopeNode* right;
    const char* str;
    int weight;
};

typedef struct RopeTree RopeTree;

struct RopeTree {
    RopeNode* root;
};

typedef struct SplitPair SplitPair;

struct SplitPair {
    RopeNode* left;
    RopeNode* right;
};

char* strdup(const char*);

RopeNode* makeRopeNode(const char* str);

RopeTree* makeRopeTree(RopeNode* root);

void printRopeNode(RopeNode* rn);

void printRopeTree(RopeTree* rt);

void debugRopeNode(RopeNode* rn, int indent);

RopeTree* concat(RopeTree* rt1, RopeTree* rt2);

char indexRope(RopeTree* rt, int idx);

char* search(RopeTree* rt, int start, int end);

SplitPair split(RopeTree* rt, int idx);

RopeTree* insert(RopeTree* rt, int idx, const char* str);

RopeTree* delete(RopeTree* rt, int start, int len);

RopeTree* copyRopeTree(RopeTree* rt);

RopeNode* copyRopeNode(RopeNode* rn);

#endif  // __ROPE__

