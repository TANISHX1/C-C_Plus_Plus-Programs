#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 10
#define MAX_WORD_HINT_LEN 80
#define MAX_TRIES 6
#define MAX_BUFFER_SIZE 256
#define DELIMITER ":"

// Style macros
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define REVERSE     "\033[7m"
#define HIDDEN      "\033[8m"
#define STRIKE      "\033[9m"

// Foreground color macros
#define FG_BLACK    "\033[30m"
#define FG_RED      "\033[31m"
#define FG_GREEN    "\033[32m"
#define FG_YELLOW   "\033[33m"
#define FG_BLUE     "\033[34m"
#define FG_MAGENTA  "\033[35m"
#define FG_CYAN     "\033[36m"
#define FG_WHITE    "\033[37m"

// Bright foreground colors
#define FG_BBLACK   "\033[90m"
#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define FG_BBLUE    "\033[94m"
#define FG_BMAGENTA "\033[95m"
#define FG_BCYAN    "\033[96m"
#define FG_BWHITE   "\033[97m"

// Helper varibles
// moves cursor by n lines up 
__uint8_t cursor_move_up = 4;

// struct to hold a word and its hint
typedef struct hidden_word
    {
    short int node_id;
    char* word;
    char* hint;
    struct hidden_word* left_node;
    struct hidden_word* right_node;
    }hidden_word;

short int counter = 0;
hidden_word* root = NULL;


hidden_word* create_node(char* word, char* hint)
    {
    hidden_word* node = (hidden_word*)malloc(sizeof(hidden_word));
    if (!node) {
        fprintf(stderr, "%sError!, hidden_word_Mem_Alloc_Failed!%s\n", FG_RED, RESET);
        return NULL;
        }
    node->word = word;
    node->hint = hint;
    node->node_id = ++counter;
    node->left_node = node->right_node = NULL;
    return node;
    }

hidden_word* insert(hidden_word* root, char* word, char* hint)
    {
    if (!root) {
        return create_node(word, hint);
        }
    int cmp = strcmp(word, root->word);

    if (cmp < 0)
        {
        // New word is alphabetically smaller, go left
        root->left_node = insert(root->left_node, word, hint);
        }
    else if (cmp > 0)
        {
        root->right_node = insert(root->right_node, word, hint);
        }
    return root;
    }

void print(hidden_word* root)
    {
    if (!root) { return; }
    print(root->left_node);
    printf("Node id [%d] \t Word : %s\t Hint: %s\n", root->node_id, root->word, root->hint);
    print(root->right_node);
    }

bool load_data()
    {

    FILE* sample_file = fopen("sample.txt", "r");
    if (!sample_file) {
        fprintf(stderr, "Sample File is Missing or InValid filed\n ");
        return 0;
        }
    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), sample_file) != NULL)
        {
        char* word = strdup(strtok(buffer, DELIMITER));
        char* hint = strdup(strtok(NULL, DELIMITER));
        root = insert(root, word, hint);
        }
    return true;

    }

hidden_word* search(hidden_word* root, int node_id)
    {
    if (!root || root->node_id == node_id) {
        return root;
        }
    hidden_word* found_node = search(root->left_node, node_id);
    if (found_node != NULL) {
        return found_node;
        }
    
        return search(root->right_node, node_id);
        

    }