#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pilha.h"
#include "lista.h"
#include "game.h"
#include "mapa.h"
#include "player.h"
#include "inventario.h"
#include "movimentacao.h"

#define TAM 12

int main(){

    Pilha*  s;
    Lista*  l;
    Player* p;
    Enemy*  e;
    int** mapa;

    startGame(mapa, TAM, p, e, s, l);

    return 0;
}