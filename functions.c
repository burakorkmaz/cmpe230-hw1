#include <stdio.h>

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

Person personList[1024];

