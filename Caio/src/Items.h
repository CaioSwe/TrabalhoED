#ifndef _Items_h_
#define _Items_h_

#include "Utils.h"
#include "Delimiters.h"

// typedef struct Weapon{
//     int code;
//     float damage;
//     float durability;
// } Weapon;

// typedef struct Shield{
//     int code;
//     float shield;
//     float durability;
// } Shield;


/*
Codes:
    1 = Poção de vida
    2 = Repelente de Monstro
    3 = Baú (pode ser mimico)
*/

typedef struct Outcome {
    bool fight;
    bool usedItem;
} Outcome;

typedef struct Item {
    int id;
    bool isMimic;

    float (*function)(float);
} Item;

void usarItem(Item* item);

const char* getItemDescription(Item* item);

#endif