#include "game.h"

void startGame(Player* p, int** mapa, int tam){
    criarMapa(p, tam);
    populaMapa(mapa, tam);
}

void gameOver(int** mapa, int tam, int end, Player* p, Enemy* e, Pilha* s, Lista* l){
    switch(end){
        case 1:
            free(p);
            free(e);
            limparPilha(s);
            limparLista(l);

            system("cls");
            printf("\tCongratulations!\n");
            printf("\nYou've beaten the dungeon!");
            limparMapa(mapa, tam);
            printf("\n\n*Crumbling noises");
            printf("\n\nAs you leave the dungeon, you hear it crumbling behind you");
            Sleep(5000);
            break;
        
        case 0: 
            free(p);
            free(e);
            limparPilha(s);
            limparLista(l);
            
            system("cls");
            printf("\tGAME OVER\n");
            printf("\nAfter arduos hours battling enemies and braving the dungeon,");
            printf("\nYou unfortunately succumb to you wounds");
            limparMapa(mapa, tam);
            printf("\nAnd the light slowly fades from your eyes");
            Sleep(5000);
            break;
    }

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