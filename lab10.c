#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
	struct Trie* letter[26];
	int count;
};

void insert(struct Trie* pTrie, char* word);
struct Trie* deallocateTrie(struct Trie* pTrie);
int numberOfOccurrences(struct Trie* pTrie, char* word);
struct Trie* createTrie();
int readDictionary(char* filename, char** pInWords);

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if(newTrie == NULL){
        return NULL;
    }
    newTrie->count = 0;
    for(int i = 0; i < 26; ++i){
        newTrie->letter[i] = NULL;
    }
    return newTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
	if (pTrie == NULL) {
        return;
    }

    int len = strlen(word);
    struct Trie* curr = pTrie;
    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= 26) {
            return;
        }
        if (curr->letter[index] == NULL) {
            curr->letter[index] = createTrie();
            if (curr->letter[index] == NULL) {
                return;
            }
        }
        curr = curr->letter[index];
    }
    curr->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
	if(pTrie == NULL) return 0;
    int len = strlen(word);
    struct Trie *curr = pTrie;
    for(int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if(curr->letter[index] == NULL) {
            return 0;
        }
        curr = curr->letter[index];
    }
    return curr->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
	if (pTrie == NULL)
        return NULL;
    for (int i = 0; i < 26; i++)
    {
        if (pTrie->letter[i] != NULL)
        {
            deallocateTrie(pTrie->letter[i]);
            pTrie->letter[i] = NULL;
        }
    }
    free(pTrie);
    return NULL;
}


// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
	FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    int num = 0;
    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[num] = (char *)malloc(strlen(word) + 1);
        strcpy(pInWords[num], word);
        num++;
    }
    fclose(file);
    return num;
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

	free(pTrie);
	return 0;
}