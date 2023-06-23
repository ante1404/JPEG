#ifndef STR_H
#define STR_H

#include "HashTable.h"

struct queue
{
    char letter;
    int priotiry;
    struct queue *next;
};

extern int j;

char *strf(char letter[], int n);
int *strf2(int freq[], char str[], int n, struct HashMap *map);
int IsIn(void *newarr, void *letter, char str[], int len_of_arr, int data_type, struct HashMap *map);
struct queue *Node(char letter, float priotiry);
struct queue *LinkedList(struct queue *root, char letter[], int priority[], int n);
struct queue *Insertion(struct queue *newnode, struct queue *root, int priority);

#endif
