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
    hidden_word* secret_node = search(root, Random_node_id);

    const char* secretword = secret_node->word;
    const char* hint = secret_node->hint;

    int length = strlen(secretword);

    char guessedword[MAX_WORD_LENGTH] = { };
    for (int i = 0;i < length;i++) {
        guessedword[i] = '_';
        }
    bool guessedletter[26] = { false };

    printf("\t\t\t%s%sWelcome to Hangman (version 2) !!!%s\n\n", FG_BCYAN, BLINK, RESET);
    printf("%sHint: %s%s\n", FG_GREEN, hint, RESET);

    short int tries = 0;

    while (tries < MAX_TRIES)
        {
        printf("\n");

        displayword(guessedword, guessedletter);
        draw(tries);

        char guess;
        printf("Enter a letter:\t");
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (guessedletter[tolower(guess) - 'a'])
            {
            printf("%s%sYou've already guessed that letter. Try again.%s\n",FG_BMAGENTA,ITALIC,RESET);
            continue;
            }

        guessedletter[guess - 'a'] = true;

        bool found = false;
        for (int i = 0; i < length; i++)
            {
            if (tolower(secretword[i]) == tolower(guess))
                {
                found = true;
                guessedword[i] = tolower(guess);
                }
            }
        if (found)
            printf("%sGood guess!\n%s",FG_GREEN,RESET);

        else
            {
            printf("%s%sSorry the letter '%c' is not in the secret Word\n%s",FG_RED,ITALIC, guess,RESET);
            tries++;
            }
        if (strcmp(secretword, guessedword) == 0)
            {
            printf("\n%s%sCongratulations! You've guessed the word: %s%s\n",FG_CYAN,BOLD, secretword,RESET);
            break;
            }
        }
    if (tries >= MAX_TRIES)
        {
        printf("\n%sSorry ,you've run out of tries.the word was: %s%s%s%s\n",FG_BRED,FG_BBLUE,UNDERLINE ,secretword,RESET);
        }

    return 0;
    }


void draw(int tries)
    {
    const char* hangman[] = {
        "  ___________", " |           |", " |           O", " |          /|\\", " |          / \\",
        " | ",           " / \\" };
    printf("\n");
    for (int i = 0; i <= tries; i++)
        {
        printf("%s\n", hangman[i]);
        }
    }