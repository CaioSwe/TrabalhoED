#ifndef _Items_h_
#define _Items_h_

#include "Utils.h"
#include "Delimiters.h"

typedef struct Weapon{
    int code;
    float damage;
    float durability;
} Weapon;

typedef struct Shield{
    int code;
    float shield;
    float durability;
} Shield;


/*
Codes:

*/

typedef struct Item {
    int id = 0;
    int amount = 0;
    int code;

    float (*function)(float);

    Vector2 current;
    Vector2 start;
    Vector2 end;
} Item;

#endif