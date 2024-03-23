#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int personCount = 0;

char *keywords[] = {
    "buy",
    "sell",
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

char *conditionWords[] = {
    "has",
    "at"
    //"less", 
    //"more", 
    //"than"
};

char *actions[] = {
    "sell",
    "buy", 
    "go"
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

// to check if the word is a conditionWord or not
bool isConditionWord(const char *word) {
    for (int i = 0; i < sizeof(conditionWords) / sizeof(conditionWords[0]); i++) {
        if (strcmp(word, conditionWords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// to check if the word is an action word or not
bool isActionWord(const char *word) {
    for (int i = 0; i < sizeof(actions) / sizeof(actions[0]); i++) {
        if (strcmp(word, actions[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isKeyword(const char *word) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isIfTrue(char *ifSentence) {
    return true;
}

char **parsing(char *tokens[], int numTokens) {
    char **sentences = malloc(sizeof(char *) * 1024);
    if (sentences == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int sentenceIndex = 0;
    int tokenIndex = 0;

    int lastTrueIfIndex = 0;

    char *sentence = malloc(sizeof(char) * 1025);
    if (sentence == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    sentence[0] = '\0'; // Initialize sentence

    if (numTokens == 1 && strcmp(tokens[0], "exit") == 0) {
        sentences[0] = strdup("exit"); // Allocate memory and copy string
        if (sentences[0] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
    
    while (tokenIndex < numTokens) {
        strcat(sentence, tokens[tokenIndex]);
        strcat(sentence, " ");

        // ali buy 2 bread if burak at mordor
        if (strcmp(tokens[tokenIndex], "buy") == 0 || strcmp(tokens[tokenIndex], "sell") == 0) {
            tokenIndex++; // Move to the next token
            if (!isNumeric(tokens[tokenIndex])) {
                printf("INVALID\n");
                break;
            }

            strcat(sentence, tokens[tokenIndex]);
            strcat(sentence, " ");

            // Check for additional items to buy
            while ((tokenIndex + 1 < numTokens) && (strcmp(tokens[tokenIndex + 1], "if") != 0) &&  
                    ((strcmp(tokens[tokenIndex], "and") != 0 || isNumeric(tokens[tokenIndex + 1])))) {
                tokenIndex++;
                if(strcmp(tokens[tokenIndex], "and") != 0) {
                    strcat(sentence, tokens[tokenIndex]);
                    strcat(sentence, " ");
                }
            }

            sentences[sentenceIndex++] = strdup(sentence); // Allocate memory and copy string
            if (sentences[sentenceIndex - 1] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            sentence[0] = '\0'; // Reset sentence for the next iteration
        }

        // ali go to ist
        else if (strcmp(tokens[tokenIndex], "go") == 0) {
            tokenIndex++; // Move to the next token

            if (strcmp(tokens[tokenIndex], "to") != 0) {
                printf("INVALID\n");
                break;
            }

            // Add "to" to sentence
            strcat(sentence, tokens[tokenIndex++]);
            strcat(sentence, " ");

            // Add location to sentence
            strcat(sentence, tokens[tokenIndex]);
            strcat(sentence, " ");
            
            if (tokenIndex + 1 < numTokens) {
                if (strcmp(tokens[tokenIndex + 1], "and") == 0) {
                    tokenIndex++;
                }
            }
            
            sentences[sentenceIndex++] = strdup(sentence); // Allocate memory and copy string
            if (sentences[sentenceIndex - 1] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            sentence[0] = '\0'; // Reset sentence for the next iteration
        }

        /* Frodo and Sam go to Bree and Frodo buy 3 map from Aragorn and Sam sell 2
           dagger to Legolas if Frodo has more than 2 ring and Legolas and Gimli at Bree
           and Frodo and Sam go to Rivendell if Aragorn has 5 map and Frodo has less than
           5 potion and Sam has 3 dagger and Frodo sell 1 potion to Arwen and Legolas
           and Gimli go to Rivendell */
        // if Frodo and Sam at Mordor
        else if (strcmp(tokens[tokenIndex], "if") == 0) {
            bool conditionFound = false;
            bool actionFound = false;
            char *condition = malloc(sizeof(char) * 10);

            int ifIndex = tokenIndex;
            int lastAndIndex = 0;

            tokenIndex++; 

            // ali buy 3 ring if burak has 3 ring and berk and bora sell 2 esek
            while (tokenIndex < numTokens) {
                
                if (strcmp(tokens[tokenIndex], "and") == 0) {
                    if (strcmp(condition, "has") == 0 && !isNumeric(tokens[tokenIndex + 1])) {
                        lastAndIndex = tokenIndex;
                        strcpy(condition, "");
                    }
                    else if (strcmp(condition, "at") == 0) {
                        lastAndIndex = tokenIndex;
                        strcpy(condition, "");
                    }
                }
                
                if (isActionWord(tokens[tokenIndex])) {
                    for (int i = ifIndex + 1; i < lastAndIndex; i++) {
                        strcat(sentence, tokens[i]);
                        strcat(sentence, " ");
                    }
                    actionFound = true;
                    tokenIndex = lastAndIndex;
                    break;
                }

                if (isConditionWord(tokens[tokenIndex])) {
                    conditionFound = true;
                    strcpy(condition, tokens[tokenIndex]);

                    if (strcmp(condition, "at") == 0) {
                        tokenIndex++;

                        if (isNumeric(tokens[tokenIndex])) {
                            printf("INVALID\n");
                            break;
                        }

                        else {
                            lastAndIndex = tokenIndex + 1;
                        }
                    }

                    // ali buy 2 bread if gandalf and gollum has 6 bread and 4 water
                    else if (strcmp(condition, "has") == 0) {
                        tokenIndex++;

                        if (strcmp(tokens[tokenIndex], "less") == 0 || strcmp(tokens[tokenIndex], "more") == 0) {
                            tokenIndex++;
                            if (strcmp(tokens[tokenIndex], "than") != 0) {
                                printf("INVALID\n");
                                break;
                            }
                        }

                        else if (!isNumeric(tokens[tokenIndex])) {
                            printf("INVALID\n");
                            break;
                        }
                    }
                }

                else {
                    tokenIndex++;
                }
            }

            if (!actionFound) {
                for (int i = ifIndex + 1; i < lastAndIndex; i++) {
                    strcat(sentence, tokens[i]);
                    strcat(sentence, " ");
                }
            }

            // SEMANTIC IF ANALYSIS


            sentences[sentenceIndex++] = strdup(sentence); // Allocate memory and copy string
            if (sentences[sentenceIndex - 1] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            sentence[0] = '\0'; // Reset sentence for the next iteration
        
        }

        tokenIndex++; // Move to the next token
    }

    free(sentence); // Free dynamically allocated memory for sentence
    return sentences;
}

//void buy(char *name[], char *items[], int quantity[])
// burak and ali buy 3 bread and 5 water
void applySentence(char *sentence) {
    //find the index of action word
    int actionIndex = 0;
    int start = 0;
    int nextStart = 0;
    int numTokens = 0;

    char *tokens[250];
    char *token = strtok(sentence, " ");
    int i = 0;

    while (token != NULL) {
        tokens[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    numTokens = i;
}

// burak buy 3 bread
void semanticAnalysis(char **sentences) {
    int sentenceIndex = 0;
    int ifIndex = 0;

    int i = 0;
    bool isIfFound = false;
    bool doesIfExist = false;

    while (sentences[i] != NULL) {
        char *sentence = malloc(sizeof(char) * 1025);
        sentence = sentences[i];

        // hold the first and second letter of the sentence
        char *firstTwoLetters = malloc(sizeof(char) * 2);
        firstTwoLetters[0] = sentence[0];
        firstTwoLetters[1] = sentence[1];

        if (strcmp(firstTwoLetters, "if") == 0) {
            ifIndex = i;
            isIfFound = true;
            doesIfExist = true;
        }

        if (ifIndex != sentenceIndex) {
            if (isIfTrue(sentences[ifIndex])) {
                for (int j = sentenceIndex; sentenceIndex < ifIndex; j++) {
                    applySentence(sentences[j]);   
                }
                sentenceIndex = ifIndex;
            }
        }

        if (isIfFound) {
            i = ifIndex;
            isIfFound = false;
        }

        else {
            i++;
        }
    }

    if (ifIndex == sentenceIndex) {
        int x;
        if (doesIfExist) {
            x = sentenceIndex + 1;
        }

        else {
            x = sentenceIndex;        
        }

        while (sentences[x] != NULL) {
            applySentence(sentences[x]);
            x++;
        }
    }
}
