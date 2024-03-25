#include <stdio.h>

void readTerminal(char *buffer, int size);
void tokenize(char *buffer, char *delim, char **tokens, int *numTokens);
void findKeyword(char *token);
void findAction(char *tokens[], int *start, int *nextStart, int *actionIndex, int numTokens);
char **parsing(char *tokens[], int numTokens);
void semanticAnalysis(char **sentences);

int start = 0;
int actionIndex = 0;
int nextStart = 0;

int main() {
    char buffer[1025];
    readTerminal(buffer, 1025);


    // print the tokenized buffer
    char *tokens[100];
    int numTokens;
    tokenize(buffer, " ", tokens, &numTokens);


    char **parsedTokens = parsing(tokens, numTokens);


    for (int i = 0; parsedTokens[i] != NULL; i++) {
        printf("%s\n", parsedTokens[i]);
    }
    semanticAnalysis(parsedTokens);

    return 0;
}
