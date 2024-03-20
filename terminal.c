#include <stdio.h>
#include <string.h>

void readTerminal(char *buffer, int size) {
    printf(">> ");
    scanf("%[^\n]", buffer);
    // fgets(buffer, size, stdin);
}

void tokenize(char *buffer, char *delim, char **tokens, int *numTokens) {
    char *token = strtok(buffer, delim);
    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        token = strtok(NULL, delim);
        i++;
    }
    *numTokens = i;
}
