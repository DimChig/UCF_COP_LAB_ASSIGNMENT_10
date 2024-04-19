#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CHARACTERS 26

// Trie structure
struct Trie
{
    struct Trie *children[NUM_CHARACTERS];
    int count;
};
struct Trie *createTrie();

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL) pTrie = createTrie();

    struct Trie * temp = pTrie;

    for (int i = 0; i < strlen(word); i++) {
        if (temp->children[word[i]-'a'] == NULL) {
            //create new node
            temp->children[word[i]-'a'] = createTrie();
        }
        //go down
        temp = temp->children[word[i]-'a'];
    }

    //word created
    temp->count += 1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    if (pTrie == NULL) return 0;
    if (strlen(word) == 0) return 0;

    struct Trie * temp = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        if (temp->children[word[i] - 'a'] == NULL) return 0;
        temp = temp->children[word[i] - 'a'];
    }
    if (temp != NULL) {
        return temp->count;
    }

    return 0;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL) return NULL;
    for (int i = 0; i < NUM_CHARACTERS; i++) {
        if (pTrie->children[i] == NULL) continue;
        deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *newNode = (struct Trie*)malloc(sizeof(struct Trie));

    for (int i = 0; i < NUM_CHARACTERS; i++) {
        newNode->children[i] = NULL;
    }
    newNode->count = 0;
    return newNode;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        printf("Can't open the file!");
        return -1;
    }

    int n = 0;
    fscanf(inputFile, "%d", &n);
    for (int i = 0; i < n; i++){
        pInWords[i] = malloc(256 * sizeof(char));
        fscanf(inputFile, "%s", pInWords[i]);
    }
    fclose(inputFile);
    return n;

}

int main(void)
{
    char *inWords[256];

    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i=0;i<numWords;++i)
    {
        printf("%s\n",inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i=0;i<numWords;i++)
    {
        insert(pTrie, inWords[i]);
    }

    // parse lineby line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i=0;i<5;i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}