#ifndef _Mapa_h_
#define _Mapa_h_

#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct player Player;

int**  criarMapa (int        );
void   populaMapa(int**,  int);
void   printMapa (int**,  int);
void   limparMapa(int**,  int);
void   ativarTrap(Player*    );

#endif