#ifndef _Movimentacao_h_
#define _Movimentacao_h_

#include "mapa.h"
#include "pilha.h"
#include "lista.h"
#include "player.h"
#include "inventario.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

typedef struct lista Lista;
typedef struct pilha Pilha;

bool moverCima         (int**, int, Pilha*, Lista*, Player*, Enemy*);
bool moverBaixo        (int**, int, Pilha*, Lista*, Player*, Enemy*);
bool moverDireita      (int**, int, Pilha*, Lista*, Player*, Enemy*);
bool moverEsquerda     (int**, int, Pilha*, Lista*, Player*, Enemy*);
void desfazerMovimento (int**, int, Pilha*, Lista*, Player*, Enemy*);

#endif