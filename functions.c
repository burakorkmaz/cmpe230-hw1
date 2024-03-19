#include <stdio.h>
#include <string.h>

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
    "go",
    "exit"
};

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
    Item items[];
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

