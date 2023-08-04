#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};

// Trie structure
struct Trie
{	
    struct TrieNode* root;
};

// Creates a new trie node
struct TrieNode* createNode()
{
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    node->count = 0;
    return node;
}



// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return;

    struct TrieNode* node = pTrie->root;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
            node->children[index] = createNode();
        node = node->children[index];
    }

    node->count++;
}

// computes the number of occurances of the word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return 0;

    struct TrieNode* node = pTrie->root;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
            return 0;
        node = node->children[index];
    }

    return node->count;
}

// deallocate the trie structure
void deallocateTrieNode(struct TrieNode* node)
{
    if (node == NULL)
        return;
    
    for (int i = 0; i < ALPHABET_SIZE; i++)
        deallocateTrieNode(node->children[i]);
    
    free(node);
}

struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie == NULL)
        return NULL;
    
    deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    trie->root = createNode();
    return trie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open the dictionary file.\n");
        exit(1);
    }

    int numWords = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

	  // parse lineby line, and insert each word to the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
	  if (pTrie != NULL)
		  printf("There is an error in this program\n");

    
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }

    return 0;
}