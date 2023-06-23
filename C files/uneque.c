#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Headers/str.h"
#include "../Headers/HashTable.h"
#include "../Headers/SaveHm.h"

int j = 0;
char *strf(char letter[], int n)
{
    char newstr[200000];
    memset(newstr, 0, sizeof(newstr));
    int newlen = 0; // Index variable for newstr array

    for (int i = 0; i < n; i++)
    {
        if (IsIn((void *)newstr, &letter[i], 0, newlen, 1, 0) == 1) // we pass 0 for arguments we dont need
        {
            newstr[newlen] = letter[i];
            newlen++;
        }
    }

    char *alocated = (char *)malloc((newlen + 1) * sizeof(char)); // Allocate space for null-terminated string
    strncpy(alocated, newstr, newlen);
    alocated[newlen] = '\0';
    printf("%s\n", alocated);

    return alocated;
}

int *strf2(int freq[], char str[], int n, struct HashMap *map)
{

    int newstr[100];
    memset(newstr, 0, sizeof(newstr));

    int newlen = 0;

    for (int i = 0; i < n; i++)
    {

        if (IsIn((void *)newstr, &freq[(int)str[i]], str, 100, 4, map) == 1)
        {
            newstr[newlen] = freq[(int)str[i]];
            newlen++;
        }
    }

    int *alocated = (int *)malloc((newlen) * sizeof(int));
    for (int i = 0; i < newlen; i++)
    {
        alocated[i] = newstr[i];
    }
    printf("\n");
    return alocated;
}

int IsIn(void *newarr, void *letter, char str[], int len_of_arr, int data_type, struct HashMap *map)
{

    if (data_type == 4)
    {

        int *int_letters = (int *)newarr; // casts newarr to int pointer and set int_letter to poinnt to that array
        for (int i = 0; i < len_of_arr; i++)
        {
            
            if (*(int *)letter == int_letters[i] && map->table[HashFunction(str[j])]->head->mark == 1) // Checks if the current cumber (letter) is in newarr(where there are no duplicates) and also if there is a number in array but the current number coresponds to a diffrient letter thus making it
            {
                j++;
                return 0;
            }
        }
        map->table[HashFunction(str[j])]->head->mark = 1;
        j++;
        return 1;
    }
    else
    {
        char char_letters[10000];
        strcpy(char_letters, (char *)newarr);
        for (int i = 0; i < len_of_arr; i++)
        {
            if (*(char *)letter == char_letters[i])
            {
                return 0;
            }
        }
        return 1;
    }
    return 1;
}

struct queue *Node(char letter, float priotiry)
{
    struct queue *newnode = (struct queue *)malloc(sizeof(struct queue));

    if (newnode == NULL)
    {
        return NULL;
    }
    newnode->letter = letter;
    newnode->priotiry = priotiry;
    newnode->next = NULL;

    return newnode;
}

struct queue *LinkedList(struct queue *root, char letter[], int priority[], int n)
{
    for (int i = 0; i < n; i++)
    {
        struct queue *newnode = Node(letter[i], priority[i]);
        root = Insertion(newnode, root, priority[i]);
    }
    
    return root;
}

struct queue *Insertion(struct queue *newnode, struct queue *root, int priority){
    struct queue *temp = NULL;

    if (root == NULL)
    {
        root = newnode;
        return root;
    }
    if (newnode->priotiry < root->priotiry)
    {
        newnode->next = root;
        root = newnode;
    }
    else
    {
        temp = root;
        while (temp->next && temp->next->priotiry <= priority)
        {
            temp = temp->next;
        }
        newnode->next = temp->next;
        temp->next = newnode;
    }

    return root;
}


