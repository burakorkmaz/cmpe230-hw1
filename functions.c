#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int personCount = 0;
bool isQuestion = false;
bool isInvalid = false;

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
                                return;
                            }
                            itemExists = true;
                            break;
                        }
                    }
                }
            }
        }
        if(!personExists){
            createPerson(&personList[personCount], name[i]);
        }
    }
}

void go(char *name[], char *location, int numSubjects) {
    for (int i = 0; i < numSubjects; i++) {
        bool personExists = false;
        for (int j = 0; j < personCount; j++) {
            if (strcmp(name[i], personList[j].name) == 0) {
                changeLocation(&personList[j], location);
                personExists = true;
            }
        }
        if (!personExists) {
            createPerson(&personList[personCount], name[i]);
            changeLocation(&personList[personCount - 1], location);
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

bool checkAtCondition(char *people[], int numPeople, char *location) {
    for (int i = 0; i < numPeople; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(people[i], personList[j].name) == 0) {
                if (strcmp(personList[j].location.name, location) != 0) {
                    return false;
                }
                else {
                    continue;
                }
            }
        }
    }
    return true;
}

bool checkHasCondition(char *people[], int numPeople, char *items[], int quantity[], int numItems) {
    for (int i = 0; i < numPeople; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(people[i], personList[j].name) == 0) {
                for (int k = 0; k < numItems; k++) {
                    bool itemExists = false;
                    for (int l = 0; l < personList[j].numItems; l++) {
                        if (strcmp(items[k], personList[j].items[l].name) == 0) {
                            if (personList[j].items[l].quantity == quantity[k]) {
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
    return true;
}

bool checkHasMoreCondition(char *people[], int numPeople, char *items[], int quantity[], int numItems) {
    for (int i = 0; i < numPeople; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(people[i], personList[j].name) == 0) {
                for (int k = 0; k < numItems; k++) {
                    bool itemExists = false;
                    for (int l = 0; l < personList[j].numItems; l++) {
                        if (strcmp(items[k], personList[j].items[l].name) == 0) {
                            if (personList[j].items[l].quantity > quantity[k]) {
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
    return true;
}

bool checkHasLessCondition(char *people[], int numPeople, char *items[], int quantity[], int numItems) {
    for (int i = 0; i < numPeople; i++) {
        for (int j = 0; j < personCount; j++) {
            if (strcmp(people[i], personList[j].name) == 0) {
                for (int k = 0; k < numItems; k++) {
                    bool itemExists = false;
                    for (int l = 0; l < personList[j].numItems; l++) {
                        if (strcmp(items[k], personList[j].items[l].name) == 0) {
                            if (personList[j].items[l].quantity < quantity[k]) {
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
    return true;
}

bool isIfTrue(char *ifSentence) {
    char **conditions = malloc(sizeof(char *) * 1024);
    int conditionIndex = 0;
    char *condition = malloc(sizeof(char) * 1024);
    int conditionStart = 0;
    int conditionNum = 0;

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

    while (i < numTokens) {
        if (strcmp(tokens[i], "if") != 0) {
            if (isConditionWord(tokens[i])) {
                char *conditionWord = tokens[i];
                if (strcmp(conditionWord, "at") == 0) {
                    strcat(condition, tokens[i++]);
                    strcat(condition, " ");
                    strcat(condition, tokens[i++]);
                    strcat(condition, " ");
                    strcat(condition, "0");
                    conditionNum++;
                }

                else if (strcmp(conditionWord, "has") == 0) {
                    char code[10] = "1"; // this variable is a code to detect the type of the if condition
                                        // set to 1 by default which states that it is "has" condition
                    if (strcmp(tokens[i + 1], "more") == 0 || strcmp(tokens[i + 1], "less") == 0) {
                        if(strcmp(tokens[i + 1], "more") == 0) {
                            strcpy(code, "2");
                        }
                        else {
                            strcpy(code, "3");
                        }

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
                    strcat(condition, code);
                    conditionNum++;
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

    /*
    printf("Conditions: \n");
    for (int i = 0; i < conditionIndex; i++) {
        printf("Condition #%d: %s\n", i + 1, conditions[i]);
    }
    */

    for (int i = 0; i < conditionNum; i++) {
        char *condition = malloc(sizeof(char) * 1024);
        condition = conditions[i];

        char conditionType = condition[strlen(condition) - 1];

        /*
         * removing the last character of the condition to get the
         * actual condition without the condition type code
         */
        condition[strlen(condition)] = '\0';
        condition[strlen(condition) - 1] = '\0';

        int conditionWordIndex = 0;

        char *tokens[100];
        int numTokens = 0;
        char *token = strtok(condition, " ");

        while (token != NULL) {
            if (isConditionWord(token)) {
                conditionWordIndex = numTokens;
            }
            tokens[numTokens] = token;
            numTokens++;
            token = strtok(NULL, " ");
        }

        char **people = malloc(sizeof(char *) * 1024);
        int numPeople = 0;

        for (int j = 0; j < conditionWordIndex; j++) {
            if (strcmp(tokens[j], "and") != 0) {
                people[numPeople] = tokens[j];
                numPeople++;
            }
        }

        if (conditionType == '0') {
            char *location = malloc(sizeof(char) * 1024);
            int conditionIndex = 0;

            strcpy(location, tokens[conditionWordIndex + 1]);

            if (checkAtCondition(people, numPeople, location)) {
                // printf("person is there\n");
            }
            else {
                return false;
            }
        }
        else if (conditionType == '1') {
            char *items[1024];
            int numItems = 0;
            int quantity[1024];
            for (int j = conditionWordIndex + 1; j < numTokens; j++) {
                if (strcmp(tokens[j], "and") != 0) {
                    if (!isNumeric(tokens[j])) {
                        items[numItems] = tokens[j];
                        numItems++;
                    }
                    else {
                        quantity[numItems] = atoi(tokens[j]);
                    }
                }
            }
            if (checkHasCondition(people, numPeople, items, quantity, numItems)) {
                // printf("the person has the items\n");
            }
            else {
                return false;
            }
        }
        else if (conditionType == '2') {
            char *items[1024];
            int numItems = 0;
            int quantity[1024]; 
            for (int j = conditionWordIndex + 3; j < numTokens; j++) {
                if (strcmp(tokens[j], "and") != 0) {
                    if (!isNumeric(tokens[j])) {
                        items[numItems] = tokens[j];
                        numItems++;
                    }
                    else {
                        quantity[numItems] = atoi(tokens[j]);
                    }
                }
            }
            if (checkHasMoreCondition(people, numPeople, items, quantity, numItems)) {
                // printf("the person has more\n");
            }
            else {
                return false;
            }
        }
        else if (conditionType == '3') {
            char *items[1024];
            int numItems = 0;
            int quantity[1024]; 
            for (int j = conditionWordIndex + 3; j < numTokens; j++) {
                if (strcmp(tokens[j], "and") != 0) {
                    if (!isNumeric(tokens[j])) {
                        items[numItems] = tokens[j];
                        numItems++;
                    }
                    else {
                        quantity[numItems] = atoi(tokens[j]);
                    }
                }
            }
            if (checkHasLessCondition(people, numPeople, items, quantity, numItems)) {
                // printf("the person has less\n");
            }
            else {
                return false;
            }
        }
        free(people);
    }

    return true;
}

int getTotalItem(char *names[], int numPeople, char *item){
    int total = 0;
    for(int i = 0 ; i < numPeople;i++ ){
        for(int j = 0; j < personCount ; j++){
            if(strcmp(names[i], personList[j].name) == 0){
                for(int k = 0; k < personList[j].numItems ; k++){
                    if(strcmp(item, personList[j].items[k].name) == 0){
                        total += personList[j].items[k].quantity;
                    }
                }
            }
        }
    }
    return total;
}

char *whereIsTheSubject(char *name) {
    for(int i = 0; i < personCount ; i++) {
        if(strcmp(personList[i].name, name) == 0) {
            return personList[i].location.name;
        }
    }
    return "NOWHERE";
}

char *whoAtLocation(char *location) {
    char* sentence = malloc(sizeof(char) * 1025);
    for(int i = 0; i < personCount ; i++) {
        if(strcmp(personList[i].location.name, location) == 0) {
            strcat(sentence, personList[i].name);
            strcat(sentence, " and ");
        }
    }
    if(strlen(sentence) == 0) {
        return "NOBODY";
    }
    else {
        sentence[strlen(sentence) - 5] = '\0';
        return sentence;
    }
}

char *totalInventory(char *name){
    char* sentence = malloc(sizeof(char) * 1025);
    bool personExists = false;
    for(int i = 0; i < personCount ; i++) {
        if(strcmp(personList[i].name, name) == 0) {
            personExists = true;
            if(personList[i].numItems == 0) {
                return "NOTHING";
            }
            for(int j = 0; j < personList[i].numItems;j++) {
                char buffer[20];
                if (personList[i].items[j].quantity != 0) {
                    sprintf(buffer , "%d",personList[i].items[j].quantity);
                    strcat(sentence, buffer);
                    strcat(sentence, " ");
                    strcat(sentence, personList[i].items[j].name);
                    strcat(sentence, " and ");
                }
            }
        }
    }
    if(!personExists) {
        return "NOTHING";
    }
    if (strlen(sentence) == 0) {
        return "NOTHING";
    }
    sentence[strlen(sentence) - 5] = '\0';
    return sentence;
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

    if(strcmp(tokens[numTokens -1], "?") == 0){
        isQuestion = true;
        if(strcmp(tokens[numTokens -3], "total") == 0){
            if(numTokens == 4){
                char **names = malloc(sizeof(char * ) * 1024);
                names[0] = tokens[0];
                printf("%d\n", getTotalItem(names ,1,tokens[numTokens -2]));
            }else{
                int i = 0;
                char **names = malloc(sizeof(char * ) * 1024);
                int numPeople = 0;
                int andCount = 0;
                while(strcmp(tokens[i], "total") != 0){
                    if(strcmp(tokens[i], "and") != 0){
                        names[numPeople] = tokens[i];
                        numPeople++;
                    }else{
                        andCount++;
                    }
                    i++;
                }
                if(numPeople > 1){
                    if(andCount + 1 != numPeople){
                        printf("INVALID\n");
                    }else{
                        printf("%d\n", getTotalItem(names, numPeople, tokens[numTokens - 2]));
                    }
                }else {
                    printf("%d\n", getTotalItem(names, numPeople, tokens[numTokens - 2]));
                }
            }
        }
        else if (strcmp(tokens[numTokens -2], "where") == 0) {
            // printf("being written in where case\n");
            if(numTokens != 3) {
                printf("INVALID\n");
            }else {
                printf("%s\n", whereIsTheSubject(tokens[0]));
            }
        }
        else if (strcmp(tokens[0], "who") == 0 && strcmp(tokens[1], "at") == 0) {
            // printf("being written in who at case\n");
            printf("%s\n", whoAtLocation(tokens[2]));
            // printf("being written is done\n");
        }
        else if(strcmp(tokens[numTokens -2], "total") == 0) {
            // printf("being written in total case\n");
            if(numTokens != 3){
                printf("INVALID\n");
            }else {
                printf("%s\n", totalInventory(tokens[0]));
            }
        }
        else{
            printf("INVALID\n");
        }
    }
    else {
        while (tokenIndex < numTokens) {
            strcat(sentence, tokens[tokenIndex]);
            strcat(sentence, " ");

            if (strcmp(tokens[tokenIndex], "buy") == 0 || strcmp(tokens[tokenIndex], "sell") == 0) {
                tokenIndex++;
                if (!isNumeric(tokens[tokenIndex])) {
                    printf("INVALID\n");
                    isInvalid = true;
                    break;
                }

                strcat(sentence, tokens[tokenIndex]);
                strcat(sentence, " ");

                // Check for additional items to buy
                while ((tokenIndex + 1 < numTokens) && (strcmp(tokens[tokenIndex + 1], "if") != 0) &&
                       ((strcmp(tokens[tokenIndex], "and") != 0 || isNumeric(tokens[tokenIndex + 1])))) {
                    tokenIndex++;
                    if (strcmp(tokens[tokenIndex], "and") != 0) {
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
                    printf("INVALID2\n");
                    isInvalid = true;
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
                        } else if (strcmp(condition, "at") == 0) {
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
                                printf("INVALID3\n");
                                isInvalid = true;
                                break;
                            } else {
                                lastAndIndex = tokenIndex + 1;
                            }
                        } else if (strcmp(condition, "has") == 0) {
                            tokenIndex++;

                            if (strcmp(tokens[tokenIndex], "less") == 0 || strcmp(tokens[tokenIndex], "more") == 0) {
                                tokenIndex++;
                                if (strcmp(tokens[tokenIndex], "than") != 0) {
                                    //printf("INVALID4\n");
                                    isInvalid = true;
                                    break;
                                }
                            } else if (!isNumeric(tokens[tokenIndex])) {
                                //printf("INVALID5\n");
                                isInvalid = true;
                                break;
                            }
                        }
                    } else {
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
    }
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
        bool personExist = false;
        for (int j = 0; j < personCount; j++) {
            if (strcmp(names[i], personList[j].name) == 0) {
                personExist = true;
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
        if(!personExist){
            createPerson(&personList[personCount], names[i]);
            return false;
        }
    }
    if (personCount == 0) {
        return false;
    }

    return true;
}

void applySentence(char *sentence) {
    int actionIndex = 0;
    int start = 0;
    int nextStart = 0;
    int numTokens = 0;

    char *tokens[250];
    char *subjects[250];
    int subjNum = 0;
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

    int andCount = 0;
    for(int i = 0; i < actionIndex ; i++){
        if(strcmp(tokens[i], "and") != 0) {
            subjects[subjIndex++] = tokens[i];
            subjNum++;
        }else{
            andCount++;
        }
    }

    if(subjIndex > 1){
        if(andCount+ 1 != subjIndex){
            printf("INVALID\n");
            isInvalid = true;
            return;
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

            for(int i = 0; i < subjIndex ; i++){
                if(!personExists(subjects[i])){
                    createPerson(&personList[personCount], subjects[i]);
                }
            }

            // multiply quantities with the number of subjects and create another array
            int newQuantities[250];
            for(int i = 0; i < itemIndex; i++){
                newQuantities[i] = quantities[i] * subjIndex;
            }

            for(int i = 0; i <subjIndex ; i++){
                if(strcmp(subjects[i],sellerName[0]) == 0){
                    isInvalid = true;
                    printf("INVALID\n");
                    break;
                }
            }

            if (!itemsExistPersonList(sellerName, items, newQuantities, 1, itemIndex)) {
                return;
            }
            else {
                buy(subjects, items, quantities, itemIndex, subjIndex);
                sell(sellerName, items, newQuantities, itemIndex, 1);
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

            if (!itemsExistPersonList(subjects, items, quantities, subjIndex, itemIndex)) {
                return;
            }
            else {
                sell(subjects, items, quantities, itemIndex, subjIndex);
            }
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

            if(!personExists(buyerName[0])){
                createPerson(&personList[personCount], buyerName[0]);
            }

            // multiply quantities with the number of subjects and create another array
            int newQuantities[250];
            for(int i = 0; i < itemIndex; i++){
                newQuantities[i] = quantities[i] * subjIndex;
            }

            for(int i = 0; i <subjIndex ; i++){
                if(strcmp(subjects[i],buyerName[0]) == 0){
                    isInvalid = true;
                    printf("INVALID\n");
                    break;
                }
            }

            if (!itemsExistPersonList(subjects, items, quantities, subjIndex, itemIndex)) {
                return;
            }
            else {
                sell(subjects, items, quantities, itemIndex, subjIndex);
                buy(buyerName, items, newQuantities, itemIndex, 1);
            }
        }
    }

    else if (strcmp(action, "go") == 0) {
        location = tokens[actionIndex + 2];
        go(subjects, location, subjIndex);
    }

    // printf("sentence after applySentence: %s\n", sentence);
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

    bool onlyOnce = true;

    while (sentences[i] != NULL) {
        char *sentence = malloc(sizeof(char) * 1025);
        sentence = sentences[i];


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

        if (ifIndex != sentenceIndex) {
            if (strlen(sentences[ifIndex]) != 2 && isIfTrue(sentences[ifIndex])) {
                for (int j = sentenceIndex; j < ifIndex; j++) {
                    applySentence(sentences[j]);
                }
                sentenceIndex = ifIndex + 1;
            }
            if (onlyOnce) {
                if(!isInvalid) {
                    printf("OK\n");
                }
                isInvalid = false;
                onlyOnce = false;
            }
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
        if (!isQuestion) {
            if(!isInvalid) {
                printf("OK\n");
            }
            isInvalid = false;
        }
        else {
            isQuestion = false;
        }
    }

    //printf("--------------------\n");
    //printPersonList();
}

bool checkSyntax(char *sentence){
    char *tokens[250];
    int numTokens = 0;
    char copySentence[300];
    strcpy(copySentence, sentence);
    char *token = strtok(copySentence, " ");
    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    numTokens = i;

    if(strcmp(tokens[0], "if") == 0){
        bool containsCondition = false;
        for (int i = 0; i < numTokens; i++) {
            if (isConditionWord(tokens[i])) {
                containsCondition = true;
                if(strcmp(tokens[i], "has") == 0){
                    if ((strcmp(tokens[i+1], "less") == 0 || strcmp(tokens[i+1], "more") == 0) && i+1 < numTokens) {
                        if (i+2 < numTokens && strcmp(tokens[i+2], "than") != 0) {
                            return false;
                        }else{
                            if(i+3 < numTokens && !isNumeric(tokens[i+3])){
                                return false;
                            }
                        }
                    } else if (!isNumeric(tokens[i+1])) {
                        return false;
                    }
                }
            }
        }
        return containsCondition;
    }
    else {
        bool containsAction = false;
        for (int i = 0; i < numTokens; i++) {
            if (isActionWord(tokens[i])) {
                containsAction = true;
                if (strcmp(tokens[i], "buy") == 0 || strcmp(tokens[i], "sell") == 0) {
                    if (!(i + 2 < numTokens && isNumeric(tokens[i + 1]) && !isNumeric(tokens[i + 2]))) {
                        return false;
                    }
                    if (isNumeric(tokens[numTokens - 1])) {
                        return false;
                    }
                }
            }
        }
        return containsAction;
    }
}
