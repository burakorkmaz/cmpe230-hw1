#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int personCount = 0;

char *keywords[] = {
    "sell",
    "buy", 
    "go", 
    "to", 
    "from", 
    "and", 
    "at", 
    "has", 
    "if", 
    "less", 
    "more", 
    "than", 
    "exit", 
    "where", 
    "total", 
    "who",
    "NOBODY", 
    "NOTHING", 
    "NOWHERE"
};

char *actions[] = {
    "sell",
    "buy", 
};

/*
"go",
"exit"
*/

typedef struct {
    char *name;
    int quantity;
} Item;

typedef struct {
    char *name;
} Location;

typedef struct {
    char *name;
    Location location; 
    Item items[250];
    int numItems;
} Person;

void printPerson(Person *person) {
    printf("Name: %s\n", person->name);
    printf("Location: %s\n", person->location.name);
    printf("Items: \n");
    for (int i = 0; i < person->items->quantity; i++) {
        printf("  %s: %d\n", person->items[i].name, person->items[i].quantity);
    }
}

void createPerson(Person *person, char *name) {
    person->name = name;
    person->location.name = "NOWHERE";
    person->numItems = 0;
    personCount++;
}

void changeLocation(Person *person, char *location) {
    person->location.name = location;
}

void findKeyword(char *input) {
    char *token = strtok(input, " ");
    while (token != NULL) {
        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
            if (strcmp(token, keywords[i]) == 0) {
                printf("Keyword: %s\n", token);
            }
        }
        token = strtok(NULL, " ");
    }
}

void findAction(char *tokens[], int *start, int *nextStart, int *actionIndex, int numTokens) {
    for (int j = *start; j < numTokens; j++) {
        for (int i = 0; i < sizeof(actions) / sizeof(actions[0]); i++) {
            if (strcmp(tokens[j], actions[i]) == 0) {
                printf("Action: %s\n", actions[i]);
                printf("Token: %s\n", tokens[j]);
                *nextStart = j + 1;
                *start = *nextStart;
                *actionIndex = j;
                return;
            }
        }
    }
    *nextStart = numTokens;
}

Person personList[1024];

void addItem(Person p, char *item, int quantity) {
    p.items[p.numItems].name = item;
    p.items[p.numItems].quantity = quantity;
    p.numItems++;
}

void buy(char *name[], char *items[], int quantity[]) {
    int numPeople = sizeof(personList) / sizeof(personList[0]);
    for (int i = 0; i < numPeople; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(name[i], personList[j].name) == 0) {
                for (int k = 0; k < personList[j].numItems; k++) {
                    if (strcmp(items[i], personList[j].items[k].name) == 0) {
                        personList[j].items[k].quantity += quantity[i];
                        return;
                    }
                    else {
                        addItem(personList[j], items[i], quantity[i]);
                        return;
                    }
                }
            }
            else {
                createPerson(&personList[personCount], name[i]);
                addItem(personList[personCount - 1], items[i], quantity[i]);
                return;
            }
        }
    }
}

// CHECK THIS FUNCTION
void sell(char *name[], char *items[], int quantity[]) {
    int numPeople = sizeof(personList) / sizeof(personList[0]);

    for (int i = 0; i < numPeople; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(name[i], personList[j].name) == 0) {
                for (int k = 0; k < personList[j].numItems; k++) {
                    if (strcmp(items[i], personList[j].items[k].name) == 0) {
                        personList[j].items[k].quantity -= quantity[i];
                        return;
                    }
                    else {
                        addItem(personList[j], items[i], quantity[i]);
                        return;
                    }
                }
            }
            else {
                createPerson(&personList[personCount], name[i]);
                addItem(personList[personCount - 1], items[i], quantity[i]);
                return;
            }
        }
    }
}

// CHECK THIS FUNCTION
void go(char *name[], char *location[]) {
    int numPeople = sizeof(personList) / sizeof(personList[0]);

    for (int i = 0; i < numPeople; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(name[i], personList[j].name) == 0) {
                changeLocation(&personList[j], location[i]);
                return;
            }
            else {
                createPerson(&personList[personCount], name[i]);
                changeLocation(&personList[personCount - 1], location[i]);
                return;
            }
        }
    }
}

// Function to check if a string is numerical
bool isNumeric(const char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }

    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return false;         }
        str++; 
    }

    return true; 
}

char **parsing(char *tokens[], int numTokens) {
    char *sentences[1024];
    int sentenceIndex = 0;
    int tokenIndex = 0;
    
    char *sentence = "";

    if (numTokens == 1 && strcmp(tokens[0], "exit") == 0) {
        sentences[0] = "exit";
        exit(0);
    }

    // ali and burak buy 2 apple
    // ali and burak buy 2 apple and 3 bread and water and bora 
    // ali and burak buy 2 apple and 3 bread from efe and bora sell 2 ring
    while (tokenIndex < numTokens) {
        sentence = strcat(strcat(sentence, " "), tokens[tokenIndex]);

        if (strcmp(tokens[tokenIndex], "buy") == 0) {
            if (!isNumeric(tokens[++tokenIndex])) {
                printf("INVALID");
                break;
            }   
            sentence = strcat(strcat(sentence, " "), tokens[tokenIndex++]); // add the quantity
                                                                            
            while (strcmp(tokens[tokenIndex], "and") != 0 || isNumeric(tokens[tokenIndex + 1])) {
                sentence = strcat(strcat(sentence, " "), tokens[tokenIndex++]); // add the item
            }
            tokenIndex++; // skip the "and"
            sentences[sentenceIndex++] = sentence;
            sentence = "";
        }
        
        else if (strcmp(tokens[tokenIndex], "sell") == 0) {
        
        }

        else if (strcmp(tokens[tokenIndex], "go") == 0) {
        
        }
        tokenIndex++;
    }

    return sentences;
}

