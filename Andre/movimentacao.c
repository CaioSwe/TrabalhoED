#include "movimentacao.h"
#include "inventario.h"
#include "mapa.h"
#include "pilha.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

bool moverCima(int** mapa, int tam, Pilha* s, Lista* l, Player* p){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){            
            if(mapa[i][j] == 1){ // Encontra o jogador no mapa
                
                // ACABOU O JOGO
                if(mapa[i-1][j] == 4){
                    mapa[i-1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 8);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i-1][j] == 5){
                    mapa[i-1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 8);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i-1][j] == 6){
                    menuItem(l);

                    mapa[i-1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 8);
                    printMapa(mapa, tam);
                }
                
                // INIMIGOS
                else if(mapa[i-1][j] == 7){
                    mapa[i-1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 8);
                    printMapa(mapa, tam);
                }

                // Posição está livre, player pode se mover livremente
                else if(mapa[i-1][j] == 0){
                    mapa[i-1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 8);
                    printMapa(mapa, tam);
                }else{
                    printf("\nThe path upwards is blocked");
                    printMapa(mapa, tam);

                    return false;
                }
                return true;
            }
        }
    }
}

bool moverBaixo(int** mapa, int tam, Pilha* s, Lista* l, Player* p){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(mapa[i][j] == 1){ // Encontra o jogador no mapa
                
                // ACABOU O JOGO
                if(mapa[i+1][j] == 4){
                    mapa[i+1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 2);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i+1][j] == 5){
                    mapa[i+1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 2);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i+1][j] == 6){
                    menuItem(l);

                    mapa[i+1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 2);
                    printMapa(mapa, tam);
                }
                
                // INIMIGOS
                else if(mapa[i+1][j] == 7){
                    mapa[i+1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 2);
                    printMapa(mapa, tam);
                }

                // Posição está livre
                else if(mapa[i+1][j] == 0){
                    mapa[i+1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 2);
                    printMapa(mapa, tam);
                }else{
                    printf("\nThe path downwards is blocked");
                    printMapa(mapa, tam);

                    return false;
                }
                return true;
            }
        }
    }
}

bool moverEsquerda(int** mapa, int tam, Pilha* s, Lista* l, Player* p){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(mapa[i][j] == 1){ // Encontra o jogador no mapa
                
                // ACABOU O JOGO
                if(mapa[i][j-1] == 4){
                    mapa[1][j-1] = 1;
                    mapa[i][j] = 0;
                    push(s, 4);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i][j-1] == 5){
                    mapa[i][j-1] = 1;
                    mapa[i][j] = 0;
                    push(s, 4);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i][j-1] == 6){
                    menuItem(l);

                    mapa[i][j-1] = 1;
                    mapa[i][j] = 0;
                    push(s, 4);
                    printMapa(mapa, tam);
                }
                
                // INIMIGOS
                else if(mapa[i][j-1] == 7){
                    mapa[i][j-1] = 1;
                    mapa[i][j] = 0;
                    push(s, 4);
                    printMapa(mapa, tam);
                }

                // Posição está livre
                else if(mapa[i][j-1] == 0){
                    mapa[i][j-1] = 1;
                    mapa[i][j] = 0;
                    push(s, 4);
                    printMapa(mapa, tam);
                }else{
                    printf("\nThe path leftwards is blocked");
                    printMapa(mapa, tam);

                    return false;
                }
                return true;
            }
        }
    }
}

bool moverDireita(int** mapa, int tam, Pilha* s, Lista* l, Player* p){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(mapa[i][j] == 1){ // Encontra o jogador no mapa
                
                // ACABOU O JOGO
                if(mapa[i][j+1] == 4){
                    mapa[1][j+1] = 1;
                    mapa[i][j] = 0;
                    push(s, 6);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i][j+1] == 5){
                    mapa[i][j+1] = 1;
                    mapa[i][j] = 0;
                    push(s, 6);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i][j+1] == 6){
                    menuItem(l);

                    mapa[i][j+1] = 1;
                    mapa[i][j] = 0;
                    push(s, 6);
                    printMapa(mapa, tam);
                }
                
                // INIMIGOS
                else if(mapa[i][j+1] == 7){
                    mapa[i][j+1] = 1;
                    mapa[i][j] = 0;
                    push(s, 6);
                    printMapa(mapa, tam);
                }

                // Posição está livre
                else if(mapa[i][j+1] == 0){
                    mapa[i][j+1] = 1;
                    mapa[i][j] = 0;
                    push(s, 6);
                    printMapa(mapa, tam);
                }else{
                    printf("\nThe path rightwards is blocked");
                    printMapa(mapa, tam);

                    return false;
                }
                return true;
            }
        }
    }
}