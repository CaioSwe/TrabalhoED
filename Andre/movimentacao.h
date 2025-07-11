#ifndef _Movimentacao_h_
#define _Movimentacao_h_

#include "inventario.h"
#include "mapa.h"
#include "pilha.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include "player.h"

typedef struct lista Lista;
typedef struct pilha Pilha;

bool moverCima         (int**, int, Pilha*, Lista*, Player*);
bool moverBaixo        (int**, int, Pilha*, Lista*, Player*);
bool moverDireita      (int**, int, Pilha*, Lista*, Player*);
bool moverEsquerda     (int**, int, Pilha*, Lista*, Player*);
void desfazerMovimento (int**, int, Pilha*                 );

#endif