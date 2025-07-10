#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "player.h"

typedef struct stats{
    float maxHealth;
    float health;
    float attack;
    float defense;
    float evasionRate;
    bool  defending;

    int gold;
}Stats;

typedef struct player{
    int   x, y;
    Stats stats;
}Player;


Player* criarPlayer(){
    Player* p = (Player*)malloc(sizeof(Player));

    p->x = 0;
    p->y = 0;
    
    p->stats.maxHealth   = 100.0;
    p->stats.health      = 100.0;
    p->stats.attack      = 1.0;
    p->stats.defense     = 0.1;
    p->stats.evasionRate = 0.1;
    p->stats.defending   = false;

    p->stats.gold        = 0;

    return p;
}