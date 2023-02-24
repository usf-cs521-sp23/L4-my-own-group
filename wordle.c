#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


void playGame(char *guess){

    printf("%s\n", guess);
    char guessWord[6];
    strcpy(guessWord, guess);
    printf("%s\n", guessWord);
    
    for(int i = 0; i < 6; i++){
        // char guess[100];
        // printf("Enter your guess: ");
        // scanf("%s", guess);

        // if(strlen(guess) != 5){
        //     printf("That is not 5 characters");
        //     continue;
        // }

        // printf("                       ");
        // for(int j = 0; j < 5; j++){
        //     if(target[j] == guess[j]){
        //         printf("o");
        //     } else if(strchr(target, guess[j]) != NULL){
        //         printf("a");
        //     } else {
        //         printf("x");
        //     }
        // }
        // puts("");

        // if(strncmp(target, guess, 5) == 0){
        //     printf("You win!\n");
        //     break;
        // }
    }
}


int main(void)
{
    char *target = "shirk"; // ok with string literal. "shirk" won't change (for now...)

    FILE *wordFile =  fopen("/usr/share/dict/words", "r");
    if(wordFile == NULL){
        perror("fopen");
    }

    FILE *outputFile = fopen("./valid_words", "w");
    if(outputFile == NULL){
        perror("fopen");
    }

    char name[100];
    printf("Hello Player!\n");
    printf("Please enter your name to play Wordle: ");
    scanf("%s", name);
    printf("Hi, %s! Let's begin...\n", name);

    // char str1[] = "Hello World";
    // char str2[13];
    // strcpy(str2, str1);
    // printf(str2);

    char line[500];
    int count = 0;
    while (fgets(line, 500, wordFile) != NULL){
        // if line is 5 letters (no punctuation)
        // then store word into a list

        if(strlen(line) == 6)
        {
            bool isWord = true;
            char str1[6];
            strcpy(str1, line);

            // add rule for isWord = false if it's a proper noun 

            for (int i = 0; i < strlen(str1) - 1; i++){
                if (str1[i] > 64 &&  str1[i] < 91){ // if it's upper case, we switch it to lower case
                    str1[i] += 32;
                } 
                if (str1[i] < 97 || str1[i] > 122) { // if it's not a lower case by now, it's not a valid word
                    isWord = false;
                } 
            }
            if (isWord){
                fputs(str1, outputFile);
                count++;
            }
        }
    }
    // printf("%d\n", count);

    fclose(wordFile);
    fclose(outputFile);

    srand(time(NULL));
    int randomNumber = (rand() % count) + 1;
    // printf("random Num is: %d\n", randomNumber);


    FILE *readFile = fopen("./valid_words", "r");
    if(readFile == NULL){
        perror("fopen");
    }

    // pick 1 word randomly 
    // set that to guess
    char guess[6];
    count = 1;
    while (fgets(line, 500, readFile) != NULL){
        if (count == randomNumber){
            strcpy(guess, line);
        }
        count++;
    }

    printf("word to guess is: %s\n", guess);
    
    fclose(readFile);

    playGame(guess);    


    // }
}