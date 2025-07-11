#ifndef _Player_h_
#define _Player_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct player Player;
typedef struct stats Stats;

Player* criarPlayer();

int     getPlayerHP   (Player*     );
void    setPlayerHP   (Player*, int);
int     getPlayerMaxHP(Player*     );

int     getPlayerGold(Player*);
void    setPlayerGold(Player*, int);

#endif