#ifndef _Game_h_
#define _Game_h_

#include "mapa.h"
#include "pilha.h"
#include "lista.h"
#include "player.h"
#include "movimentacao.h"

#include <stdio.h>
#include <stdlib.h>

void startGame  (int**, int, Player*, Enemy*, Pilha*, Lista*     );
void restartGame(int**, int, Player*, Enemy*, Pilha*, Lista*     );
void gameOver   (int**, int, int, Player*, Enemy*, Pilha*, Lista*);
void gameMenu   (int**, int, Pilha*, Lista*, Player*, Enemy*     );
void moveMenu   (int**, int, Pilha*, Lista*, Player*, Enemy*     );

#endif