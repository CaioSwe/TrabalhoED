#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pilha.h"
#include "lista.h"
#include "inventario.h"
#include "movimentacao.h"
#include "mapa.h"
#include "player.h"

#define TAM 12

int main(){
    int** mapa = criarMapa(TAM);
    Pilha*  s = criarPilha();
    Lista*  l = criaLista();
    Player* p = criarPlayer();
    
    populaMapa(mapa, TAM);
    printMapa (mapa, TAM);

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
      
    return 0;
}