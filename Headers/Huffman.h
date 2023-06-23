#ifndef HUFFMAN_H
#define HUFFMAN_H
#define MAXNUMOFNODES 100000

#include "../Headers/str.h"

struct HuffmanNode
{
    char letter;
    int frequency;
    struct HuffmanNode *right;
    struct HuffmanNode *left;
    struct HuffmanNode *next;
};

struct HuffmanNode *huffmanNode(char letter, int frequency, struct HuffmanNode *left, struct HuffmanNode *right);
struct HuffmanNode *huffmanTree(struct HuffmanNode *root);
struct HuffmanNode *HuffInsertion(struct HuffmanNode *newnode, struct HuffmanNode *root, int priority);
struct HuffmanNode *Deltion(struct HuffmanNode *root);
void printHuffmanTree(struct HuffmanNode *root, int level);
char **constructHuffmanCodes(struct HuffmanNode *root, int level);
void BubbleSort(int arr[], int n);
void GenerateMsg(struct HuffmanNode *root, char *code[], char orgMSg[]);
void writeBit(FILE *file, int bit, int i);

#endif