#include <stdio.h>

void readTerminal(char *buffer, int size);
void tokenize(char *buffer, char *delim, char **tokens, int *numTokens);

int main() {
    char buffer[1025];
    readTerminal(buffer, 1025);
    //printf("%s\n", buffer);

    // print the tokenized buffer
    char *tokens[100];
    int numTokens;
    tokenize(buffer, " ", tokens, &numTokens);
    /*
    for (int i = 0; i < numTokens; i++) {
        printf("%s\n", tokens[i]);
    }
    */

    //printf("Number of tokens: %d\n", numTokens);

    return 0;
}
