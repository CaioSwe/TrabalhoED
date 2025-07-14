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

void encounterEnemy (Lista*, Player*, Enemy*);
void attackEnemy    (Player*, Enemy*        );
void defendFromEnemy(Player*, Enemy*        );
bool tryEscape      (                       );
void enemyTurn      (Player*, Enemy*        );
bool isPlayerDead   (Player*, Enemy*        );
bool isEnemyDead    (Player*, Enemy*        );

#endif