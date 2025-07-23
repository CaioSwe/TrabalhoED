#ifndef _Mapa_h_
#define _Mapa_h_

#include "game.h"
#include "pilha.h"
#include "lista.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct player Player;

int**  criarMapa  (Player*, int                                    );
void   populaMapa (int**, int                                      );
void   printMapa  (int**, int                                      );
void   limparMapa (int**, int                                      );
void   ativarTrap (int**, int, Player*, Enemy*, Pilha*, Lista*     );
void   gameOver   (int**, int, int, Player*, Enemy*, Pilha*, Lista*);

#endif