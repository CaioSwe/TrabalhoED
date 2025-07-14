#ifndef _Inventario_h_
#define _Inventario_h_

#include "lista.h"
#include "player.h"
#include "combate.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct lista Lista;
typedef struct player Player;

void menuItem        (Lista*, Player*, Enemy*     );
int  menuInventario  (                            );
void coletarItem     (int, Lista*, Player*, Enemy*);
int  itemChoice      (                            );
void abrirInventario (Lista*, Player*             );
void usarItem        (Lista*, Player*             );
void descartarItem   (Lista*                      );
void inspecionarItem (Lista*                      );

#endif