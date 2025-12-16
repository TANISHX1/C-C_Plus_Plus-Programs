#include "header.h"

// Function to display the current state of the word
void displayword(const char word[], const bool guessed[])
{
    printf("Word : ");
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (guessed[tolower(word[i]) - 'a'])
        {
            printf("%c ", word[i]);
        }
        else
        {
            printf("_ ");
        }
    }
    printf("\n");
}
// Function to draw the hangman
void draw(int tries);

// main code
int main()
    {
    
    load_data();

    // selecting a random number
    srand(time(NULL));
    int Random_node_id = rand() % counter;
    // int Random_node_id = 3;
    printf("Random id :%d\n",Random_node_id);
    hidden_word* secret_node = search(root, Random_node_id);

    printf("Node id [%d] \tWord: %s \thint: %s\n", secret_node->node_id, secret_node->word, secret_node->hint);

    const char *secretword = wordlist[index].word;
    const char *hint = wordlist[index].hint;

    // int length = strlen(secretword);

    // char guessedword[MAX_WORD_LENGTH] = {0};
    // bool guessedletter[26] = {false};

    // printf("Welcome to Hangman!!!\n");
    // printf("Hint: %s\n", hint);

    // int tries = 0;

    // while (tries < MAX_TRIES)
    // {
    //     printf("\n");

    //     displayword(guessedword, guessedletter);
    //     draw(tries);

    //     char guess;
    //     printf("Enter a letter:\t");
    //     scanf(" %c", &guess);
    //     guess = tolower(guess);

    //     if (guessedletter[tolower(guess) - 'a'])
    //     {
    //         printf("You've already guessed that letter. "
    //                "Try again.\n");
    //         continue;
    //     }

    //     guessedletter[guess - 'a'] = true;

    //     bool found = false;
    //     for (int i = 0; i < length; i++)
    //     {
    //         if (tolower(secretword[i]) == tolower(guess))
    //         {
    //             found = true;
    //             guessedword[i] = tolower(guess);
    //         }
    //     }
    //     if (found)
    //         printf("Good guess!\n");

    //     else
    //     {
    //         printf("Sorry the letter '%c' is not in the "
    //                "secret Word\n",
    //                guess);
    //         tries++;
    //     }
    //     if (strcmp(secretword, guessedword) == 0)
    //     {
    //         printf("\nCongratulations! You've guessed the "
    //                "word: %s\n",
    //                secretword);
    //         break;
    //     }
    // }
    // if (tries >= MAX_TRIES)
    // {
    //     printf("\nSorry ,you've run out of tries.the word was: %s\n", secretword);
    // }

    return 0;
}


void draw(int tries)
{
    const char *hangman[] = {
        "  ___________", " |           |", " |           O", " |          /|\\", " |          / \\",
        " | ",           " / \\"};
    printf("\n");
    for (int i = 0; i <= tries; i++)
    {
        printf("%s\n", hangman[i]);
    }
}