#ifndef MONEYTOR_LIST_PRIVATE_H
#define MONEYTOR_LIST_PRIVATE_H

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
