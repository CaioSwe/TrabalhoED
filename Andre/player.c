#include "player.h"

typedef struct stats{
    int   maxHealth;
    int   health;
    int   attack;
    int   repelent;
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

int   getPlayerHP(Player* p            ) { return p->stats.health;   }
void  setPlayerHP(Player* p, int health) { p->stats.health = health; }

int   getPlayerGold(Player* p          ) { return p->stats.gold; }
void  setPlayerGold(Player* p, int gold) { p->stats.gold = gold; }

int   getPlayerRepelent(Player* p            ) { return p->stats.repelent;   }
void  setPlayerRepelent(Player* p, int amount) { p->stats.repelent = amount; }

int   getPlayerMaxHP(Player* p) { return p->stats.maxHealth; }