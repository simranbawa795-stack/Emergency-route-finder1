#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

// Step 1: Define a structure for a single node (letter box) in the tree
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_end_of_word;
};

// Helper function to create a brand new empty node
struct TrieNode *createNode() {
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    newNode->is_end_of_word = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Step 2: Function to insert a word into the tree structure
void insertWord(struct TrieNode *root, const char *word) {
    struct TrieNode *currentNode = root;

    for (int i = 0; word[i] != '\0'; i++) {
        // Convert character to array index (e.g., 'a' -> 0, 'b' -> 1)
        int index = word[i] - 'a';

        // If the path for this letter does not exist, create a new link
        if (currentNode->children[index] == NULL) {
            currentNode->children[index] = createNode();
        }
        // Move deeper into the tree branch
        currentNode = currentNode->children[index];
    }
    // Mark the last letter node as a complete valid word
    currentNode->is_end_of_word = true;
}

// Helper function: Uses Depth First Search (DFS) to collect all letters down a branch
void collectWords(struct TrieNode *node, char *currentPrefix, int length) {
    if (node == NULL) return;

    // If we reach a node marked as the end of a word, print what we have typed so far
    if (node->is_end_of_word) {
        currentPrefix[length] = '\0';
        printf("  -> %s\n", currentPrefix);
    }

    // Traverse all 26 possible alphabetical child branches recursively
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            currentPrefix[length] = i + 'a'; // Convert index back to character
            collectWords(node->children[i], currentPrefix, length + 1);
        }
    }
}

// Step 3: Function to find the prefix and display autocomplete suggestions
bool printSuggestions(struct TrieNode *root, const char *query) {
    struct TrieNode *currentNode = root;
    int length = strlen(query);

    // Walk down the tree following the characters typed by the user
    for (int i = 0; i < length; i++) {
        int index = query[i] - 'a';

        // If the prefix path doesn't exist, there are no matching words
        if (currentNode->children[index] == NULL) {
            return false;
        }
        currentNode = currentNode->children[index];
    }

    // Allocate a local buffer string array to keep track of collected letters during DFS
    char prefixBuffer[100];
    strcpy(prefixBuffer, query);

    // Start collecting and printing all remaining branches down this specific path
    collectWords(currentNode, prefixBuffer, length);
    return true;
}

// Clean up memory function to prevent memory leaks before exiting
void freeTrie(struct TrieNode *root) {
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }
    free(root);
}

// =====================================================================
// Step 4: Main Interactive Simulation Loop
// =====================================================================
int main() {
    struct TrieNode *root = createNode();

    // A clean sample dataset of words loaded into the local engine
    char *wordDatabase[] = {
        "amazon", "apple", "app", "application", "aptitude",
        "algorithm", "array", "binary", "blockchain", "code",
        "coding", "computer", "google", "instagram", "python",
        "programming", "project"
    };

    int databaseSize = sizeof(wordDatabase) / sizeof(wordDatabase[0]);

    // Feed all words into our data structure
    for (int i = 0; i < databaseSize; i++) {
        insertWord(root, wordDatabase[i]);
    }

    printf("==========================================================\n");
    printf("      🚀 REAL-TIME SEARCH AUTOCOMPLETE ENGINE (C) 🚀      \n");
    printf("==========================================================\n");
    printf("Loaded %d reference words into the dynamic Trie structure.\n", databaseSize);
    printf("Type letters to get live suggestions (Type 'exit' to quit).\n\n");

    char userInput[50];
    while (1) {
        printf("Start typing a word: ");
        scanf("%49s", userInput);

        // Break the infinite loop if the user inputs exit
        if (strcmp(userInput, "exit") == 0) {
            printf("Exiting project application. Goodbye!\n");
            break;
        }

        printf("💡 Suggestions for '%s':\n", userInput);

        bool found = printSuggestions(root, userInput);
        if (!found) {
            printf("  ❌ No matching word suggestions found.\n");
        }
        printf("----------------------------------------------------------\n");
    }

    // Clean dynamic memory allocations from RAM
    freeTrie(root);
    return 0;
}
