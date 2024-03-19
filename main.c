#include <stdio.h>

void readTerminal(char *buffer, int size);
void tokenize(char *buffer, char *delim, char **tokens, int *numTokens);
void findKeyword(char *token);
void findAction(char *tokens[], int *start, int *nextStart, int *actionIndex, int numTokens);

int start = 0;
int actionIndex = 0;
int nextStart = 0;

int main() {
    char buffer[1025];
    readTerminal(buffer, 1025);
    //printf("%s\n", buffer);

    // print the tokenized buffer
    char *tokens[100];
    int numTokens;
    tokenize(buffer, " ", tokens, &numTokens);

    /*
    printf("Token: ");
    for (int i = 0; i < numTokens; i++) {
        printf("%s ", tokens[i]);
    }
    printf("\n");
    */

    while (nextStart < numTokens) {
        findAction(tokens, &start, &nextStart, &actionIndex, numTokens);
        //start = nextStart;
    }

    printf("Start: %d\n", start);
    printf("Action Index: %d\n", actionIndex);
    printf("Next Start: %d\n", nextStart);

    //printf("Number of tokens: %d\n", numTokens);

    return 0;
}
