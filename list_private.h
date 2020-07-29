#ifndef MONEYTOR_LIST_PRIVATE_H
#define MONEYTOR_LIST_PRIVATE_H

// CR: (DC) Think what here is private (intended to be seen by list module developers only) and what is
// CR: (DC) public (intended to be seen by other developers)
// CR: (DC) A general rule: implementation details are private (such as node-related stuff), and API
// CR: (DC) details are public (such as the LIST definition)
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
