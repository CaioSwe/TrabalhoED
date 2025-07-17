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

    Pilha*  s = criarPilha();
    Lista*  l = criaLista();
    Player* p = criarPlayer();
    Enemy*  e = criarEnemy();
    int** mapa = NULL;
    criarMapa(&mapa, p, TAM);

    startGame(mapa, TAM, p, e, s, l);

    return 0;
}