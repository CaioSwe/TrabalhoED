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
    int** mapa = criarMapa(p, TAM);
    
    populaMapa(mapa, TAM);
    printMapa (mapa, TAM);

    
    moverCima    (mapa, TAM, s, l, p);
    moverEsquerda(mapa, TAM, s, l, p); 
    moverBaixo   (mapa, TAM, s, l, p); 
    moverBaixo   (mapa, TAM, s, l, p); 
    moverBaixo   (mapa, TAM, s, l, p); 
    moverBaixo   (mapa, TAM, s, l, p);

    abrirInventario(l, p);
    
    moverDireita (mapa, TAM, s, l, p); 
    moverDireita (mapa, TAM, s, l, p); 
    moverDireita (mapa, TAM, s, l, p); 
    
    abrirInventario(l, p);

    desfazerMovimento(mapa, TAM, s, l, p);
      
    return 0;
}