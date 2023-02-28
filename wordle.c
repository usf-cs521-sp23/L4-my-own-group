#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const char *wordFilePath = "/usr/share/dict/words";
const char *validWordFilePath = "./valid_words";
const int WORD_SIZE = 5;

int playGame(char *guess){

    printf("The word to guess is: %s\n", guess);
    char guessWord[6];
    strncpy(guessWord, guess, 5);
    guessWord[5] = '\0';
    // printf("%s\n", guessWord);
    
    char userGuess[6];
    int usedChar[26] = { 0 };
    bool containsInvalidChars = false;
    bool notValidWord = true;
    // keep track of wins and losses and print , read to file **
    for(int i = 6; i > 0; i--){
        while((strlen(userGuess) != 5) || containsInvalidChars || notValidWord){
            containsInvalidChars = false;
            notValidWord = true;
            printf("Enter your guess: ");
            scanf("%s", userGuess);
            printf("Your guess is: %s\n", userGuess);
            if(strlen(userGuess) != 5){
                printf("That is not 5 characters, please try again... \n");
            }
            if(strlen(userGuess) == 5){
                // printf("valid length");
                for (int j = 0; j < 5; j++){
                    if (userGuess[j] < 97 || userGuess[j] > 122){
                        printf("this guess contains invalid characters, try again...\n");
                        containsInvalidChars = true;
                        break;
                    }
                }
                userGuess[5] = '\0';
            }

            // try to see if theres a better way to check if its a valid word **
            if(strlen(userGuess) == 5 && !containsInvalidChars){
                FILE *validWordFile = fopen(validWordFilePath, "r");
                char line[6];
                // check if it's a valid word against dictionary
                while(fgets(line, 6, validWordFile) != NULL){
                    // need to fix this logic
                    if(strcmp(line, userGuess) == 0){
                        printf("it's a valid word\n");
                        notValidWord = false;
                        break;
                    } 
                }
                if (notValidWord) {
                    printf("Your guess is not a word in my dictionary, try again... \n");
                }
                fclose(validWordFile);
            }  
        }

        // printf("Your guess is: %s\n", userGuess);
        // printf("userGuess Length is: %d\n", strlen(userGuess));

        // input letters in guess into array of used letters


        // print letters used
        int usedCharLength = sizeof(usedChar) / sizeof(int);
        for (int j = 0; j < 5; j++){
            int index = userGuess[j] - 97;
            // printf("userGuess[] is %s\n", userGuess[j]);
            // printf("Index is: %d\n", index);
            if(usedChar[index] == 0){
                usedChar[index] = 1;
            }
        }
        
        // print out guess word result
        printf("               ");
        for(int j = 0; j < 5; j++){
            // might still be not working right
            // printf("GuessWord length is : %d \n", strlen(guessWord));
            // printf("GuessWord: %s\n", guessWord);
            // printf("userGuess: %s\n", userGuess);
            if(guessWord[j] == userGuess[j]){
                printf("\033[0;32m");
                printf("o");
                printf("\033[0;37m");
            } else if(strchr(guessWord, userGuess[j]) != NULL){
                printf("\033[0;33m");
                printf("!");
                printf("\033[0;37m");
            } else {
                printf("\033[0;31m");
                printf("x");
                printf("\033[0;37m");
            }
        }
        puts("");

        printf("The characters used so far: \n");
        for (int j = 0; j < usedCharLength; j++){
            if (usedChar[j] == 1){
                printf("%c ", j + 97);
            }
        }
        puts("");

        if(strncmp(guessWord, userGuess, 5) == 0){   
            return 0;
        } else {
            strcpy(userGuess, "");
            printf("You have %d guess(es) remaining!\n", i - 1);
        }
    }
    return 1;
}

int generateWordFile(int *count){
    FILE *wordFile =  fopen(wordFilePath, "r");
    if(wordFile == NULL){
        perror("fopen");
        return 1;
    }

    FILE *outputFile = fopen(validWordFilePath, "w");
    if(outputFile == NULL){
        perror("fopen");
        return 1;
    }

    char line[500];
    int count_copy = *count;
    while (fgets(line, 500, wordFile) != NULL){
        if(strlen(line) == 6)
        {
            bool isWord = true;
            char str1[6];
            strcpy(str1, line);
            // printf("line: %s\n", line);

            if (str1[0] > 64 &&  str1[0] < 91){ // added rule for isWord = false if it's a proper noun 
                    isWord = false;
            } else {
                for (int i = 0; i < strlen(str1) - 1; i++){
                    if (str1[i] > 64 &&  str1[i] < 91){ // if it's upper case, we switch it to lower case
                        str1[i] += 32;
                    } 
                    if (str1[i] < 97 || str1[i] > 122) { // if it's not a lower case by now, it's not a valid word
                        isWord = false;
                    }
                }
            }
            if (isWord){
                fputs(str1, outputFile);
                count_copy++; 
                fflush(outputFile);
            } 
        }
        fflush(wordFile);
    }
    *count = count_copy;
    return 0;

    fclose(wordFile);
    fclose(outputFile);
}

void selectRandomWord(char *wordToGuess, int *count){
    int totalValidWords = *count;
    srand(time(NULL));
    int randomNumber = (rand() % totalValidWords) + 1;
    // printf("randomNumber is %d\n", randomNumber);
    FILE *readFile = fopen(validWordFilePath, "r");
    if(readFile == NULL){
        perror("fopen");
    }

    char word[WORD_SIZE + 1];
    word[5] = '\0';
    // strncpy(word, wordToGuess, WORD_SIZE);
    // printf("This is word: %s\n", word);

    // pick 1 word randomly 
    // set that to wordToGuess
    char line[10];
    int currentLine = 1;
    while (fgets(line, 10, readFile) != NULL){
        if (currentLine == randomNumber){
            strncpy(word, line, WORD_SIZE);
            // printf("This is word: %s\n", word);
            // test = selectedGuessWord;
            // strcpy(wordToGuess, selectedGuessWord);
            break;
        }
        currentLine++;
    }
    strcpy(wordToGuess, word);
    // printf("wordToGuess is %s\n", wordToGuess);
    fclose(readFile);
}


int main(void)
{
    char wordToGuess[WORD_SIZE + 1];
    wordToGuess[WORD_SIZE] = '\0';
    int totalValidWords = 0;
    char playerName[100];

    printf("Welcome to Wordle!\n");
    printf("Please wait while we load up...\n");
    printf("---------------------------------------\n");

    if (generateWordFile(&totalValidWords) != 0 || totalValidWords == 0){
        printf("Program is quitting in error...\n");
        return 1;
    }
    // printf("total valid words: %d\n", totalValidWords);
    // printf("---------------------------------------\n");
    printf("Hello!\n");
    printf("Please enter your name to play Wordle: ");
    // need to handle case where name is a string separated by space
    scanf("%s", playerName);

    // fgets(name, 100, stdin);
    printf("Hi, %s! Let's begin...\n", playerName);
    // printf("length of player name: %d\n", strlen(playerName));
    printf("---------------------------------------\n");

    bool play = true;
    while (play){
        // generate word
        selectRandomWord(wordToGuess, &totalValidWords);
        char response[10];
        int result = playGame(wordToGuess); 
        if(result == 0){
            char command[100];
                sprintf(command, "cowsay Congratulations %s, you win!", playerName);
                system(command);

        } else {
            printf("Oops, looks like you lose this one, %s.\n", playerName);
            printf("The correct answer is %s.\n", wordToGuess);
        }

        printf("want to play again? [Y/N] --> ");
        scanf("%s", response);
        if (strcmp(response, "N") == 0 || strcmp(response, "n") == 0){
            play = false;
        }
    }
       
    printf("Thanks for playing Wordle, %s! Have a good day!\n", playerName);
    
}