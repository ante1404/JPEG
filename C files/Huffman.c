#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/str.h"
#include "../Headers/SaveHm.h"
#include "../Headers/HashTable.h"
#include "../Headers/Huffman.h"

//Indexes for the GenerateMsg function
int m = 0;
long long u = 0;
/*
int main()
{
    int n = 0;
    //Obvious
    struct HashMap *map = ReadHm("../Binary data/book2.bin");
    struct HuffmanNode *star = NULL;
    struct HuffmanNode *parrent = NULL;
    //We load the data from the hash map about the file and make the leafe nodes for out tree//
    char *newstr = (char *)malloc(256 * sizeof(char));
    long int *newfreq = (long int *)malloc(256 * sizeof(long int));
    for (int i = 0; i < map->size; i++)
    {
        struct bin *b = map->table[i];
        if (b->head)
        {
            struct Node *temp = b->head;
            while (temp)
            {
                newstr[n] = temp->letter;
                newfreq[n] = temp->freq;
                parrent = huffmanNode(newstr[n], newfreq[n], NULL, NULL);
                star = HuffInsertion(parrent, star, parrent->frequency);
                n++;
                printf("letter %c and freq = %d\n", temp->letter, temp->freq);
                temp = temp->next;
            }
        }
    }

    char **code = NULL;
    star = huffmanTree(star);
    printHuffmanTree(star, 0);
    code = constructHuffmanCodes(star, 0);

    for (int i = 0; i < 256; i++)
    {
        if (code[i] != NULL)
        {
            printf("%c: %s\n", i, code[i]);
        }
    }
    GenerateMsg(star, code, "../Uncompressed data/book2.txt");
    Delete(map);
    free(newstr);
    free(newfreq);

    return 0;
}
*/
struct HuffmanNode *huffmanNode(char letter, int frequency, struct HuffmanNode *left, struct HuffmanNode *right)
{

    struct HuffmanNode *newnode = (struct HuffmanNode *)malloc(sizeof(struct HuffmanNode));
    if (newnode == NULL)
    {
        return NULL;
    }
    newnode->letter = letter;
    newnode->frequency = frequency;
    newnode->left = left;
    newnode->right = right;
    newnode->next = NULL;

    return newnode;
}

struct HuffmanNode *huffmanTree(struct HuffmanNode *root)
{
    struct HuffmanNode *parrent = NULL;
    struct HuffmanNode *right = NULL;
    struct HuffmanNode *left = NULL;

    while (root->next)
    {
        int newfreq = root->frequency + root->next->frequency;                      // 2 lowest frequencys
        left = huffmanNode(root->letter, root->frequency, root->left, root->right); // Make copyes of the first 2 nodes
        right = huffmanNode(root->next->letter, root->next->frequency, root->next->left, root->next->right);
        parrent = huffmanNode('*', newfreq, left, right); // Create new node that is a parent to the first 2
        // printf("parrent:%c -> %d: Left child %d -> %c: Right child: %d -> %c\n", parrent->letter, parrent->frequency, parrent->left->frequency, parrent->left->letter, parrent->right->frequency, parrent->right->letter);
        root = Deltion(root);                                    // Delete 2 first nodes
        root = HuffInsertion(parrent, root, parrent->frequency); // Insert the node back to the pq
    }

    return root;
}

struct HuffmanNode *HuffInsertion(struct HuffmanNode *newnode, struct HuffmanNode *root, int priority)
{
    //This function is effeectivly the same as the one in the uneque.c but the diffrience is the data structire we are working with
    struct HuffmanNode *temp = NULL;

    if (root == NULL)
    {
        root = newnode;
        return root;
    }
    if (newnode->frequency < root->frequency)
    {
        newnode->next = root;
        root = newnode;
    }
    else
    {
        temp = root;
        while (temp->next && temp->next->frequency <= priority)
        {
            temp = temp->next;
        }
        newnode->next = temp->next;
        temp->next = newnode;
    }

    return root;
}

struct HuffmanNode *Deltion(struct HuffmanNode *root)
{
    //deletes the first 2 nodes in a pq of huffman pq
    struct HuffmanNode *temp = NULL;

    for (int i = 0; i < 2; i++)
    {
        if (root->next == NULL)
        {
            free(root);
            root = NULL;
            return root;
        }
        else
        {
            temp = root->next;
            free(root);
            root = temp;
        }
    }
    return root;
}

void printHuffmanTree(struct HuffmanNode *root, int level)
{
    if (root != NULL)
    {
        printHuffmanTree(root->right, level + 1);
        // Print node
        for (int i = 0; i < level; i++)
        {
            printf("\t");
        }
        printf("%c (%d)\n", root->letter, root->frequency);
        printHuffmanTree(root->left, level + 1);
    }
}

char **constructHuffmanCodes(struct HuffmanNode *root, int level)
{
    static char *code[256] = {0}; // Array to store codes
    static char buffer[256];      // Buffer for temporary code storage

    if (root->letter != '*')
    {
        code[root->letter] = strdup(buffer); // Copy buffer to code
        return code;
    }

    // Traverse right subtree
    buffer[level] = '1';
    buffer[level + 1] = '\0';
    constructHuffmanCodes(root->right, level + 1);

    // Traverse left subtree
    buffer[level] = '0';
    buffer[level + 1] = '\0';
    constructHuffmanCodes(root->left, level + 1);

    return code;
}

void GenerateMsg(struct HuffmanNode *root, char *code[], char orgMSg[])
{

    char *str = NULL;
    long file_size;
    FILE *DNA = fopen(orgMSg, "r");

    fseek(DNA, 0, SEEK_END);
    file_size = ftell(DNA);
    fseek(DNA, 0, SEEK_SET);

    str = malloc(file_size + 1);

    fread(str, 1, file_size, DNA);
    fclose(DNA);
    unsigned long long int n = strlen(str);
    printf("%lld\n", n);
    int *msg = (int *)malloc((1000000) * sizeof(int));
    FILE *hun = fopen("../Compressed data/cbook2.bin", "wb");
    /*Now for each charater in original message we traverse the inner loop to find its 
    coresponding binary code that was constructed by huffman tree and we converti it to int and store it to an msg array
    The reason we allocate only 1 milion bytes and free and repeate is because you can allocate gbs of 
    data to do it all at one unless you have supercomputer*/
    for (unsigned long long int i = 0; i < n; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if (code[j] != NULL)
            {
                if (str[i] == j)
                {
                    for (int lj = 0; code[j][lj] != '\0'; lj++)
                    {
                        u++;
                        if (m == 1000000)
                        {
                            for (int i = 0; i < m; i++)
                            {
                                //this function does the intiger conversion to is binary reprenstation and writes it to a fille thus compressing the data
                                writeBit(hun, msg[i],0);
                            }
                            free(msg);
                            msg = (int *)malloc((1000000) * sizeof(int));
                            m = 0;
                        }

                        if (code[j][lj] == '0')
                        {
                            msg[m] = 0;
                            m++;
                        }
                        else if (code[j][lj] == '1')
                        {
                            msg[m] = 1;
                            m++;
                        }
                        
                    }
                    break;
                }
            }
        }
    }
    if (u % 8 != 0)
    {
        /*this checks if we have enough bytes to write the last byte or we have to pad it.
        Because we cant write 123.4 bytes to disck*/
        for (int i = 0; i < m; i++)
        {
            writeBit(hun, msg[i], 0);
        }
        printf("test uga");
        writeBit(hun, msg[m], 1);
    }else{
        for (int i = 0; i < m; i++)
        {
            writeBit(hun, msg[i], 0);
        }
    }
    
    
    
    printf("\n%lld\n", u);
    fclose(hun);
    free(str);
    free(msg);
}

void writeBit(FILE *file, int bit, int i)
{
    /*This function writes bytes to a file, each 8 bits we write 1 byte, if the 
    inut is 1,0,0,1,1,0,1,0 then the byte we write will be 10011010, this 
    obviusly saves space as this is the whole point of it insted of just writing
    10000000 and 00000000 for each of those inputs, thus compression would not be possible*/
    static int currentByte = 0;
    static int bitCount = 8;

    if (bitCount == 8)
    {
        currentByte = 0;
    }

    currentByte |= (bit << bitCount);
    bitCount--;

    if (bitCount == 0)
    {
        currentByte = currentByte >> 1;
        fwrite(&currentByte, sizeof(unsigned char), 1, file);
        bitCount = 8;
    }
    if (i == 1 && bitCount != 8)
    {
        /*The argument i acts like a boolean, where 1 means we still have few bites but not 
        enough to have 1 8 bit byte, so we pad the remaning spaces with zeros, you might think this can 
        be a probelm if we have a letter on location 000 with huffman code and when we 
        get to the last byte the last the function in decoding program will itterate the tree
        with false data (padded zeros) and we might get the, but condition in DecompressSF h >= NumOfLetters keeps that from happening*/
        printf("Number of Bites/BiteCount -> %d\n", bitCount);
        while (bitCount != 0)
        {
            currentByte |= (bit << bitCount);
            bitCount--;
        }
        currentByte = currentByte >> 1;
        fwrite(&currentByte, sizeof(unsigned char), 1, file);
    }
    
}