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

int start = 0;
int actionIndex = 0;
int nextStart = 0;
extern bool isQuestion;

int main() {
    char *keywords[] = {
        "NOBODY",
        "NOTHING",
        "NOWHERE"
    };

    while (1) {
        bool doesContainKeyword = false;
        char buffer[1025];
        readTerminal(buffer, 1025);

        char *tokens[100];
        int numTokens;
        tokenize(buffer, " ", tokens, &numTokens);

        char **parsedTokens = parsing(tokens, numTokens);
        if (isQuestion) {
            isQuestion = false;
            continue;
        }

        bool syntaticallyTrue = true;
        for (int i = 0; parsedTokens[i] != NULL; i++) {
            // printf("%s\n", parsedTokens[i]);
            if(!checkSyntax(parsedTokens[i])){
                // printf("question sentences are regarded as invalid\n");
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
        if(syntaticallyTrue) {
            // printf("enters syntacticlyTrue if\n");
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
        else{
            printf("INVALID\n");
        }
    }
    return 0;
}
