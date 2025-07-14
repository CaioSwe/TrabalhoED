#ifndef _Game_h_
#define _Game_h_

#include "mapa.h"
#include "pilha.h"
#include "lista.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>

void startGame(Player*, int**, int);
void gameOver (int**, int, int, Player*, Enemy*, Pilha*, Lista*);
void moveMenu (                   );

#endif