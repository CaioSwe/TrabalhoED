#include "game.h"
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>

void startGame(int** mapa, int tam){
    criarMapa(tam);
    populaMapa(mapa, tam);
}

void gameOver(int** mapa, int tam){
    system("cls");
    printf("\tGAMO OVER\n");
    printf("\nAfter arduos hours battling enemies and braving the dungeon,");
    printf("\nYou unfortunately succumb to you wounds");

    for(int i = 0; i < tam; i++){
            free(mapa[i]);
    }free(mapa);
    printf("\nAnd the light slowly fades from your eyes");
    Sleep(5000);

    int opc;
    do{
        system("cls");
        printf("\nPress 1 to restart the game");
        printf("\nPress 0 to exit the game\n");
        scanf("%d", &opc);
        if     (opc == 1) {}// RECOMEÇA O JOGO
        else if(opc == 0) exit(1);
        else              {printf("\nInvalid choice"); opc = 10;}
    }while(opc == 10);
}

void moveMenu(int** mapa, int tam){
    printMapa(mapa, tam);
}