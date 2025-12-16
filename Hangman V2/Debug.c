#include "header.h"
#define LINE "==============================="
#define FIELD_SEP "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
int main()
    {
    puts("\t\t\tThis is Debug Code\n");
    printf("%s\n\n", FIELD_SEP);
    printf("[+]  Data Storing [%sBinary Tree%s] \n < %s Implementation %s >\n\n", FG_BGREEN, RESET,LINE,LINE);
    load_data();
    print(root);
    
    // Random number chicking 
    printf("%s\n\n", FIELD_SEP);
    puts("[+]  Random Word Pick\n\n");
    srand(time(NULL));
    int Random_node_id = rand() % counter;
    // int Random_node_id = 3;
    printf("Random id : %d\n\n",Random_node_id);
    hidden_word* secret_node = search(root, Random_node_id);

    printf("Node id [%d] \tWord: %s \thint: %s\n", secret_node->node_id, secret_node->word, secret_node->hint);


    return 0;
    }