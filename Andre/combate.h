#ifndef _Combate_h_
#define _Combate_h_

#include "game.h"
#include "lista.h"
#include "player.h"
#include "inventario.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void encounterEnemy  (Lista*, Player*, Enemy*, Pilha*, int**, int);
void attackEnemy     (Player*, Enemy*                            );
void defendFromEnemy (Player*, Enemy*, Pilha*, Lista*, int**, int);
bool tryEscape       (Player*                                    );
void enemyTurn       (Player*, Enemy*, Pilha*, Lista*, int**, int);
bool isEnemyDead     (Player*, Enemy*                            );
bool isPlayerDead    (Player*, Enemy*, Pilha*, Lista*, int**, int);

#endif