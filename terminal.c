/*
 * READ FIRST main.c
 * This file contains functions about terminals
 */
#include <stdio.h>
#include <string.h>

//this function helps us to reading input from terminal
void readTerminal(char *buffer, int size) {
    printf(">> ");
    // scanf("%[^\n]", buffer);
    fgets(buffer, size, stdin);
    int len = strlen(buffer);
    buffer[len - 1] = '\0';
}
//this function tokenize the input and gives us the words array
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
