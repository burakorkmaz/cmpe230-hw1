#include <stdio.h>


typedef struct {
    char *name;
} Item;

typedef struct {
    char *name;
} Location;

typedef struct {
    char *name;
    Location *location; 
    Item items[];
} Person;
