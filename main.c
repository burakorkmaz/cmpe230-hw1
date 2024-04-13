/*  PLEASE READ THIS FILE FIRST
 * WE HAVE 3 FILE : main.c functions.c terminal.c
 * main.c is about some basic INVALID checks and parsing the sentences
 * functions.c contains parsing functions, person lists, semantic functions such as buy, sell, go and if analysis
 * terminal.c contains reading the input from terminal and tokenize it
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readTerminal(char *buffer, int size);
void tokenize(char *buffer, char *delim, char **tokens, int *numTokens);
void findKeyword(char *token);
void findAction(char *tokens[], int *start, int *nextStart, int *actionIndex, int numTokens);
char **parsing(char *tokens[], int numTokens);
void semanticAnalysis(char **sentences);
bool checkSyntax(char *sentence);
void samePersonExists(int *samePerson, char *sentence);
void checkIfNumeric(int *containsNumeric, char *sentence);

int start = 0;
int actionIndex = 0;
int nextStart = 0;
extern bool isQuestion;

int main() {
    char *keywords[] = { // if input contains one of them, return invalid
            "NOBODY",
            "NOTHING",
            "NOWHERE"
    };

    while (1) {
        int samePerson = 0;
        int containsNumeric = 0;
        bool doesContainKeyword = false;
        char buffer[1025];
        readTerminal(buffer, 1025);
        //if input is empty return invalid
        if (strlen(buffer) == 0) {
            printf("INVALID\n");
            continue;
        }

        char *tokens[1025];
        int numTokens;
        tokenize(buffer, " ", tokens, &numTokens);
        char **parsedTokens = parsing(tokens, numTokens);//after parsing operations, separate the sentences
        if (isQuestion) {
            isQuestion = false;
            continue;
        }

        //check sentences whether they have syntax error or not
        bool syntaticallyTrue = true;
        for (int i = 0; parsedTokens[i] != NULL; i++) {
            printf("%s\n", parsedTokens[i]);
            if(!checkSyntax(parsedTokens[i])){
                syntaticallyTrue = false;
                break;
            }
        }

        for (int i = 0; i < numTokens; i++) {
            for (int j = 0; j < 3; j++) {
                if (strcmp(tokens[i], keywords[j]) == 0) {
                    doesContainKeyword = true;
                    break;
                }
            }
        }

        if (doesContainKeyword) {
            printf("INVALID\n");
            continue;
        }

        // check if the people's names contain numbers
        // copying the parsedTokens to a new array because of the checkIfNumeric function
        char **parsedTokensFirstCopy = (char **)malloc(sizeof(char *) * 1025);
        for (int i = 0; parsedTokens[i] != NULL; i++) {
            parsedTokensFirstCopy[i] = (char *)malloc(sizeof(char) * 1025);
            strcpy(parsedTokensFirstCopy[i], parsedTokens[i]);
        }

        for (int i = 0; parsedTokensFirstCopy[i] != NULL; i++) {
            if (containsNumeric) break;
            checkIfNumeric(&containsNumeric, parsedTokensFirstCopy[i]);
        }
        if (containsNumeric) {
            printf("INVALID\n");
            continue;
        }

        // check for the same person but first copy the parsedTokens to a new array
        // copying the parsedTokens to a new array because of the samePersonExists function
        char **parsedTokensSecondCopy = (char **)malloc(sizeof(char *) * 1025);
        for (int i = 0; parsedTokens[i] != NULL; i++) {
            parsedTokensSecondCopy[i] = (char *)malloc(sizeof(char) * 1025);
            strcpy(parsedTokensSecondCopy[i], parsedTokens[i]);
        }

        for (int i = 0; parsedTokensSecondCopy[i] != NULL; i++) {
            if (samePerson) break;
            samePersonExists(&samePerson, parsedTokensSecondCopy[i]);
        }

        if (samePerson) {
            printf("INVALID\n");
            continue;
        }
        //if sentences are syntatically true, then continue the semantics
        if(syntaticallyTrue) {
            // check if the first token is "exit"
            if (numTokens == 1 && strcmp(parsedTokens[0], "exit") == 0) {
                // free memory before exiting
                for (int i = 0; parsedTokens[i] != NULL; i++) {
                    free(parsedTokens[i]);
                }
                free(parsedTokens);
                break;
            }
            semanticAnalysis(parsedTokens);
        }
        else {
            printf("INVALID\n");
        }
    }
    return 0;
}