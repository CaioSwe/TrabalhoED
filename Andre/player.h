#ifndef _Player_h_
#define _Player_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct player Player;
typedef struct enemy Enemy;
typedef struct stats Stats;

/* FUNÇÕES REFERENTES AO PLAYER */
Player* criarPlayer();
Enemy*  criarEnemy ();

float getPlayerHP(Player*       );
void  setPlayerHP(Player*, float);

int   getPlayerGold(Player*     );
void  setPlayerGold(Player*, int);

int   getPlayerRepelent(Player*     );
void  setPlayerRepelent(Player*, int);

float getPlayerAttack(Player*       );
void  setPlayerAttack(Player*, float);

int*  getPlayerPos(Player*          );
void  setPlayerPos(Player*, int, int);

float getPlayerMaxHP(Player*);
float getPlayerDefense(Player*);

/* FUNÇÕES REFERENTES AO INIMIGO */
float getEnemyHP(Enemy*       );
void  setEnemyHP(Enemy*, float);

float getEnemyAttack(Enemy*);
float getEnemyDefense(Enemy*);

bool  getEnemyMimic(Enemy*      );
void  setEnemyMimic(Enemy*, bool);

#endif