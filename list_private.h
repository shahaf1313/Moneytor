#ifndef MONEYTOR_LIST_PRIVATE_H
#define MONEYTOR_LIST_PRIVATE_H

typedef struct list list_t;
typedef struct node node_t;
typedef  list_t* LIST;

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
