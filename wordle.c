#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


int playGame(char *guess){

    printf("The word to guess is: %s\n", guess);
    char guessWord[6];
    strncpy(guessWord, guess, 5);
    guessWord[5] = '\0';
    printf("%s\n", guessWord);
    
    char userGuess[6];
    int usedChar[26] = { 0 };
    bool containsInvalidChars = false;
    bool notValidWord = true;

    for(int i = 6; i > 0; i--){
        while((strlen(userGuess) != 5) || containsInvalidChars || notValidWord){
            containsInvalidChars = false;
            notValidWord = true;
            printf("Enter your guess: ");
            scanf("%s", userGuess);
            printf("user guess: %s\n", userGuess);
            if(strlen(userGuess) != 5){
                printf("That is not 5 characters, please try again... \n");
            }
            if(strlen(userGuess) == 5){
                printf("valid length");
                for (int j = 0; j < 5; j++){
                    if (userGuess[j] < 97 || userGuess[j] > 122){
                        printf("this guess contains invalid characters, try again...\n");
                        containsInvalidChars = true;
                        break;
                    }
                }
                userGuess[5] = '\0';
            }
            if(strlen(userGuess) == 5 && !containsInvalidChars){
                FILE *validWordFile = fopen("./valid_words", "r");
                char line[10];
                // check if it's a valid word against dictionary
                while(fgets(line, 10, validWordFile) != NULL){
                    if(strstr(line, userGuess) == 0){
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

        

        printf("Your guess is: %s\n", userGuess);
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
                printf("o");
            } else if(strchr(guessWord, userGuess[j]) != NULL){
                printf("!");
            } else {
                printf("x");
            }
            // puts("");
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
            printf("You have %d guess(es) remaining!\n", i);
        }
    }

    return 1;
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
    char wordToGuess[6];
    count = 1;
    while (fgets(line, 10, readFile) != NULL){
        if (count == randomNumber){
            strncpy(wordToGuess, line, 5);
        }
        count++;
    }

    printf("word to guess is: %shello\n", wordToGuess);
    for(int i = 0; i < strlen(wordToGuess); i++){
        // printf("WordToGuess[%d] is %c\n", i, wordToGuess[i]);
    }
    printf("guess length is: %d\n", strlen(wordToGuess));
    wordToGuess[5] = '\0';
    for(int i = 0; i < strlen(wordToGuess); i++){
        // printf("WordToGuess[%d] is %c\n", i, wordToGuess[i]);
    }
    // printf("guess length is: %d\n", strlen(wordToGuess));
    
    fclose(readFile);

    bool play = true;
    while (play){
        char response;
        int result = playGame(wordToGuess); 
        if(result == 0){
            printf("You win!\n");
        } else {
            printf("you lose\n");
        }
        printf("want to play again? [Y/N] --> ");

        // to fix
        scanf(" %c", response);
        printf("this is response: %c\n", response);
    
        if (response != 89 || response != 121){

            play = false;
        }
    }
       
    printf("done with game, closing....\n");
    

    //

}