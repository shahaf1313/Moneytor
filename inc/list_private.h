#ifndef MONEYTOR_LIST_PRIVATE_H
#define MONEYTOR_LIST_PRIVATE_H

// CR: (SE) This line is duplicated from moneytorDefinitions.h in order to allow separate debugging printer of each
// CR: (SE) module (list is not a part of moneytor and vice versa).
#define DEBUG_PRINT(...) {printf(__VA_ARGS__); printf("\n");}

typedef struct node node_t;

struct node {
    struct node* prev;
    struct node* next;
    void* data;
};

struct list {
    int (*memoryReleaseFunc)(void*);
    char* (*getDataNameFunc)(void*);
    node_t* first;
    struct node* last;
    int length;
};

#endif //MONEYTOR_LIST_PRIVATE_H
