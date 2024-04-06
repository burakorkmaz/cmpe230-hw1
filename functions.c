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
};

char *actions[] = {
    "sell",
    "buy",
    "go"
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
    Item items[250];
    int numItems;
} Person;

void printPerson(Person *person) {
    printf("Name: %s\n", person->name);
    printf("Location: %s\n", person->location.name);
    printf("numItems: %d\n", person->numItems);
    for (int i = 0; i < person->numItems; i++) {
        printf("Item: %s\n", person->items[i].name);
        printf("Quantity: %d\n", person->items[i].quantity);
    }
    printf("\n");
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

void findAction(char *tokens[], int *actionIndex, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        for (int j = 0; j < sizeof(actions) / sizeof(actions[0]); j++) {
            if (strcmp(tokens[i], actions[j]) == 0) {
                *actionIndex = i;
                return;
            }
        }
    }
}

Person personList[1024];

void addItem(Person *p, char *item, int quantity) {
    p->items[p->numItems].name = item;
    p->items[p->numItems].quantity = quantity;
    p->numItems++;
}

void buy(char *name[], char *items[], int quantity[], int numItems, int numSubjects) {
    for (int i = 0; i < numSubjects; i++) {
        bool personExists = false;
        if (personCount == 0) {
            createPerson(&personList[personCount], name[i]);
            personExists = true;
            for (int j = 0; j < numItems; j++) {
                addItem(&personList[personCount - 1], items[j], quantity[j]);
            }
            continue;
        }

        for (int j = 0; j < personCount; j++) {
            if (strcmp(name[i], personList[j].name) == 0) {
                personExists = true;
                for (int l = 0; l < numItems; l++) {
                    bool itemExists = false;
                    for (int k = 0; k < personList[j].numItems; k++) {
                        if (strcmp(items[l], personList[j].items[k].name) == 0) {
                            personList[j].items[k].quantity += quantity[l];
                            itemExists = true;
                            break;
                        }
                    }
                    if (!itemExists) {
                        addItem(&personList[j], items[l], quantity[l]);
                    }
                }
            }
        }

        if (!personExists) {
            createPerson(&personList[personCount], name[i]);
            for (int k = 0; k < numItems; k++) {
                addItem(&personList[personCount - 1], items[k], quantity[k]);
            }
        }
    }
}

void sell(char *name[], char *items[], int quantity[], int numItems, int numSubjects) {
    for (int i = 0; i < numSubjects; i++) {
        bool personExists = false;
        if (personCount == 0) {
            return;
        }
        for (int j = 0; j < personCount; j++) {
            if (strcmp(name[i], personList[j].name) == 0) {
                personExists = true;
                for (int l = 0; l < numItems; l++) {
                    bool itemExists = false;
                    for (int k = 0; k < personList[j].numItems; k++) {
                        if (strcmp(items[l], personList[j].items[k].name) == 0) {
                            // quantity check 
                            if (personList[j].items[k].quantity >= quantity[l]) {
                                personList[j].items[k].quantity -= quantity[l];
                            }
                            else {
                                printf("OK\n");
                                return;
                            }
                            itemExists = true;
                            break;
                        }
                    }
                    if (!itemExists) {
                        printf("OK\n");
                        return;
                    }
                }
            } 
        }
        printf("OK\n");
    }
}

void go(char *name[], char *location, int numSubjects) {
    for (int i = 0; i < numSubjects; i++) {
        bool personExists = false;
        for (int j = 0; j < personCount; j++) {
            if (strcmp(name[i], personList[j].name) == 0) {
                changeLocation(&personList[j], location);
            }
        }
        if (!personExists) {
            createPerson(&personList[personCount], name[i]);
            changeLocation(&personList[personCount - 1], location);
        }
    }
    printf("OK\n");
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

bool personExists(char *name) {
    for (int i = 0; i < personCount; i++) {
        if (strcmp(name, personList[i].name) == 0) {
            return true;
        }
    }
    return false;
}

void conditionWordIndex(char *tokens[], int *conditionIndex, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        if (isConditionWord(tokens[i])) {
            *conditionIndex = i;
            return;
        }
    }
}

// THIS FUNCTION IS NOT FINISHED
bool isIfTrue(char *ifSentence) {
    char **conditions = malloc(sizeof(char *) * 1024);
    int conditionIndex = 0;
    char *condition = malloc(sizeof(char) * 1024);
    int conditionStart = 0;

    char *tokens[100];
    int numTokens = 0;
    char *token = strtok(ifSentence, " ");
    
    // calculate the number of tokens using a while loop
    while (token != NULL) {
        tokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }

    int i = 0;

    // if ali has 3 bread and 5 ring and burak at mordor
    while (i < numTokens) {
        if (strcmp(tokens[i], "if") != 0) {
                if (isConditionWord(tokens[i])) {
                    char *conditionWord = tokens[i];  
                    if (strcmp(conditionWord, "at") == 0) {
                        i++; // location index
                        for (int j = conditionStart; j <= i; j++) {
                            strcat(condition, tokens[j]);
                            strcat(condition, " ");
                        }
                    }

                    // ali buy 3 bread if burak has 3 ring
                    // if bora has more than 4 ring and burak has 2 efe 
                    else if (strcmp(conditionWord, "has") == 0) {
                        if (strcmp(tokens[i + 1], "more") == 0 || strcmp(tokens[i + 1], "less") == 0) {
                            strcat(condition, tokens[i++]);
                            strcat(condition, " ");
                            strcat(condition, tokens[i++]);
                            strcat(condition, " ");
                            strcat(condition, tokens[i++]);
                            strcat(condition, " ");
                        } 
                        while ((i < numTokens) && (strcmp(tokens[i], "and") != 0 || isNumeric(tokens[i + 1]))) {
                            strcat(condition, tokens[i]);
                            strcat(condition, " ");
                            i++;
                        }
                    }
                    conditions[conditionIndex++] = strdup(condition);
                    conditionStart = i + 1;
                    condition[0] = '\0';
                }
                else {
                    strcat(condition, tokens[i]);
                    strcat(condition, " ");
                }
        }
        else if (strcmp(tokens[i], "if") == 0) {
            conditionStart = i + 1;
        }

        i++;
    }
    printf("Conditions: \n");
    for (int i = 0; i < conditionIndex; i++) {
        printf("Condition #%d: %s\n", i + 1, conditions[i]);
    }
    
    return true;
}

char **parsing(char *tokens[], int numTokens) {
    char **sentences = malloc(sizeof(char *) * 1024);
    int sentenceIndex = 0;
    int tokenIndex = 0;

    int lastTrueIfIndex = 0;

    char *sentence = malloc(sizeof(char) * 1025);
    sentence[0] = '\0'; 

    if (numTokens == 1 && strcmp(tokens[0], "exit") == 0) {
        sentences[0] = strdup("exit"); 
        exit(0);
    }

    while (tokenIndex < numTokens) {
        strcat(sentence, tokens[tokenIndex]);
        strcat(sentence, " ");

        if (strcmp(tokens[tokenIndex], "buy") == 0 || strcmp(tokens[tokenIndex], "sell") == 0) {
            tokenIndex++; 
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

            sentences[sentenceIndex++] = strdup(sentence); 
            sentence[0] = '\0'; 
        }

        else if (strcmp(tokens[tokenIndex], "go") == 0) {
            tokenIndex++; 

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

            sentences[sentenceIndex++] = strdup(sentence); 
            sentence[0] = '\0'; 
        }

        // if ali has 3 bread
        else if (strcmp(tokens[tokenIndex], "if") == 0) {
            bool conditionFound = false;
            bool actionFound = false;
            char *condition = malloc(sizeof(char) * 10);

            int ifIndex = tokenIndex;
            int lastAndIndex = 0;

            tokenIndex++;

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

            if (tokenIndex >= numTokens - 1) {
                lastAndIndex = numTokens;
            }

            if (!actionFound) {
                for (int i = ifIndex + 1; i < lastAndIndex; i++) {
                    strcat(sentence, tokens[i]);
                    strcat(sentence, " ");
                }
                if (lastAndIndex == 0) {
                    for (int i = ifIndex + 1; i < numTokens; i++) {
                        strcat(sentence, tokens[i]);
                        strcat(sentence, " ");
                    }
                }
            }


            sentences[sentenceIndex++] = strdup(sentence); 
            sentence[0] = '\0'; 
        }

        tokenIndex++; 
        if ((tokenIndex == numTokens) && !(strcmp(sentence, "") == 0)) {
            sentences[sentenceIndex++] = strdup(sentence); 
        }
    }

    free(sentence); 
    return sentences;
}

bool doesFromExist(char **tokens, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        if (strcmp(tokens[i], "from") == 0) {
            return true;
        }
    }
    return false;
}

bool doesToExist(char **tokens, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        if (strcmp(tokens[i], "to") == 0) {
            return true;
        }
    }
    return false;
}

void indexOfFrom(char **tokens, int *fromIndex, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        if (strcmp(tokens[i], "from") == 0) {
            *fromIndex = i;
            return;
        }
    }
}

void indexOfTo(char **tokens, int *toIndex, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        if (strcmp(tokens[i], "to") == 0) {
            *toIndex = i;
            return;
        }
    }
}

bool itemsExistPersonList(char *names[], char *items[], int quantities[], int numNames, int numItems) {
    for (int i = 0; i < numNames; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(names[i], personList[j].name) == 0) {
                for (int k = 0; k < numItems; k++) {
                    bool itemExists = false;
                    for (int l = 0; l < personList[j].numItems; l++) {
                        if (strcmp(items[k], personList[j].items[l].name) == 0) {
                            if (personList[j].items[l].quantity >= quantities[k]) {
                                itemExists = true;
                                break;
                            }
                        }
                    }
                    if (!itemExists) {
                        return false;
                    }
                }   
            }
        }
    }
    if (personCount == 0) {
        return false;
    }

    return true;
}

void applySentence(char *sentence) {
    // find the index of action word
    printf("sentence in applySentence: %s\n", sentence);
    int actionIndex = 0;
    int start = 0;
    int nextStart = 0;
    int numTokens = 0;

    char *tokens[250];
    char *subjects[250];
    int subjIndex = 0;
    char *items[250];
    int itemIndex = 0;
    int quantities[250];
    char *location;
    char *token = strtok(sentence, " ");
    int i = 0;

    while (token != NULL) {
        tokens[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    numTokens = i;

    findAction(tokens, &actionIndex, numTokens);
    char *action = tokens[actionIndex];

    for(int i = 0; i < actionIndex ; i++){
        if(strcmp(tokens[i], "and") != 0) {
            subjects[subjIndex++] = tokens[i];
        }
    }

    if (strcmp(action, "buy") == 0) {
        if (!doesFromExist(tokens, numTokens)) {
            for(int j = actionIndex + 1; j < numTokens; j++) {
                if (strcmp(tokens[j], "and") != 0)  {
                    if (!isNumeric(tokens[j])) {
                        items[itemIndex++] = tokens[j];
                    }
                    else {
                        quantities[itemIndex] = atoi(tokens[j]);
                    }
                }
            }
            buy(subjects, items, quantities, itemIndex, subjIndex);
        }

        else {
            int fromIndex;
            indexOfFrom(tokens, &fromIndex, numTokens);
            for(int j = actionIndex + 1; j < fromIndex; j++) {
                if (strcmp(tokens[j], "and") != 0)  {
                    if (!isNumeric(tokens[j])) {
                        items[itemIndex++] = tokens[j];
                    }
                    else {
                        quantities[itemIndex] = atoi(tokens[j]);
                    }
                }
            }
            char *seller = tokens[fromIndex + 1];
            char *sellerName[10];
            sellerName[0] = seller;

            if (!itemsExistPersonList(sellerName, items, quantities, 1, itemIndex)) {
                printf("OK\n");
                return;
            }
            else {
                buy(subjects, items, quantities, itemIndex, subjIndex);
                sell(sellerName, items, quantities, itemIndex, 1);
            }
        }
    }

    else if (strcmp(action, "sell") == 0) {
        if (!doesToExist(tokens, numTokens)) {
            for(int j = actionIndex + 1; j < numTokens; j++) {
                if (strcmp(tokens[j], "and") != 0)  {
                    if (!isNumeric(tokens[j])) {
                        items[itemIndex++] = tokens[j];
                    }
                    else {
                        quantities[itemIndex] = atoi(tokens[j]);
                    }
                }
            }
            sell(subjects, items, quantities, itemIndex, subjIndex);
        }

        else {
            int toIndex;
            indexOfTo(tokens, &toIndex, numTokens);
            for(int j = actionIndex + 1; j < toIndex; j++) {
                if (strcmp(tokens[j], "and") != 0) {
                    if (!isNumeric(tokens[j])) {
                        items[itemIndex++] = tokens[j];
                    }
                    else {
                        quantities[itemIndex] = atoi(tokens[j]);
                    }
                }
            }

            char *buyer = tokens[toIndex + 1];
            char *buyerName[10];
            buyerName[0] = buyer;
            if (!itemsExistPersonList(subjects, items, quantities, subjIndex, itemIndex)) {
                printf("OK\n");
                return;
            }
            else {
                sell(subjects, items, quantities, itemIndex, subjIndex);
                buy(buyerName, items, quantities, itemIndex, 1);
            }
        }
    }

    else if (strcmp(action, "go") == 0) {
        location = tokens[actionIndex + 2];
        go(subjects, location, subjIndex);
    }

    printf("sentence after applySentence: %s\n", sentence);
}

void printPersonList() {
    for (int i = 0; i < personCount; i++) {
        printPerson(&personList[i]);
    }
}

void semanticAnalysis(char **sentences) {
    int sentenceIndex = 0;
    int ifIndex = 0;

    int i = 0;
    bool isIfFound = false;
    bool doesIfExist = false;

    while (sentences[i] != NULL) {
        char *sentence = malloc(sizeof(char) * 1025);
        sentence = sentences[i];

        printf("sentence: %s\n", sentence);

        // hold the first and second letter of the sentence
        char *firstTwoLetters = malloc(sizeof(char) * 2);
        firstTwoLetters[0] = sentence[0];
        firstTwoLetters[1] = sentence[1];

        if (firstTwoLetters[0] == 'i' && firstTwoLetters[1] == 'f') {
            ifIndex = i;
            isIfFound = true;
            doesIfExist = true;
        }
        free(firstTwoLetters);

        // find out why the sentence changes. here and also in the applySentence function
        if (ifIndex != sentenceIndex) {
            printf("if sentence: %s\n", sentences[ifIndex]);
            if (strlen(sentences[ifIndex]) != 2 && isIfTrue(sentences[ifIndex])) {
                for (int j = sentenceIndex; j < ifIndex; j++) {
                    applySentence(sentences[j]);
                }
                sentenceIndex = ifIndex + 1;
            }
            printf("if sentence after: %s\n", sentences[ifIndex]);
        }

        if (isIfFound) {
            i = ifIndex + 1;
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
    printf("--------------------\n");
    printPersonList();
}
