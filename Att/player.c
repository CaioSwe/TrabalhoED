#include "player.h"

typedef struct stats{
    int   repelent;
    float maxHealth;
    float health;
    float attack;
    float defense;
    float dodge;
    float evasionRate;
    bool  defending;

    int gold;
}Stats;
typedef struct player{
    int   x, y;
    Stats stats;
}Player;

typedef struct enemy{
    bool  mimic;
    float maxHealth;
    float health;
    float attack;
    float defense;
    bool  defending;
}Enemy;

Player* criarPlayer(){
    Player* p = (Player*)malloc(sizeof(Player));

    p->x = 1;
    p->y = 1;
    
    p->stats.maxHealth   = 100.0;
    p->stats.health      = 100.0;
    p->stats.attack      = 25.0;
    p->stats.repelent    = 0;
    p->stats.defense     = 0.05;
    p->stats.dodge       = 0.1;
    p->stats.evasionRate = 0.1;
    p->stats.defending   = false;

    p->stats.gold        = 0;

    return p;
}

Enemy* criarEnemy(){
    Enemy* e = (Enemy*)malloc(sizeof(Enemy));

    e->mimic       = false;
    e->maxHealth   = 75.0;
    e->health      = 75.0;
    e->attack      = 10.0;
    e->defense     = 0.05;
    e->defending   = false;

    return e;
}

/* FUNÇÕES REFERENTES AO PLAYER */
float getPlayerHP(Player* p              ) { return p->stats.health;   }
void  setPlayerHP(Player* p, float health) { p->stats.health = health; }

int   getPlayerGold(Player* p          ) { return p->stats.gold; }
void  setPlayerGold(Player* p, int gold) { p->stats.gold = gold; }

int   getPlayerRepelent(Player* p            ) { return p->stats.repelent;   }
void  setPlayerRepelent(Player* p, int amount) { p->stats.repelent = amount; }

float getPlayerAttack(Player* p              ) { return p->stats.attack;   }
void  setPlayerAttack(Player* p, float attack) { p->stats.repelent = attack; }

float getPlayerMaxHP  (Player* p) { return p->stats.maxHealth; }
float getPlayerDefense(Player* p) { return p->stats.defense;   }

int*  getPlayerPos(Player* p              ) {
    int* pos = (int*)malloc(2*sizeof(int));
    pos[0] = p->x; 
    pos[1] = p->y; 
    
    return pos; 
}
void  setPlayerPos(Player* p, int x, int y) {
    p->x = x; 
    p->y = y;
}

/* FUNÇÕES REFERENTES AO INIMIGO */
float getEnemyHP(Enemy* e              ) { return e->health;   }
void  setEnemyHP(Enemy* e, float health) { e->health = health; }

float getEnemyAttack (Enemy* e) { return e->attack;  }
float getEnemyDefense(Enemy* e) { return e->defense; }

bool  getEnemyMimic(Enemy* e            ) { return e->mimic;  }
void  setEnemyMimic(Enemy* e, bool mimic) { e->mimic = mimic; }