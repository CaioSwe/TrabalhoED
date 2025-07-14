#include "game.h"

void startGame(int** mapa, int tam, Player* p, Enemy* e, Pilha* s, Lista* l){
    system("cls");
    printf("\tWelcome to the game!");
    printf("\n\n\tBy");
    printf("\nAndre Felipe Ijiri Ribeiro\tCaio Sweiwer de Carvalho");
    printf("\n\t:)");
    Sleep(2500);
    system("cls");

    printf("\tOne day, you decide to go adventuring, despite you mother's worries");
    printf("'\nIt's dangerous to go alone! Take this' She tells you, handing you an old but trusty sword");
    printf("\nAs well an old and dirty bag\n\n");
    system("pause");
    system("cls");

    mapa = criarMapa(p, tam);
    printf("Soon enough, you're entering the infamous Dungeon of Danger, right beside your house");
    printf("\n'How convinient!' You think to yourself\n\n");
    system("pause");
    system("cls");

    populaMapa(mapa, tam);
    printf("As you enter the dungeon, you can hear all the monsters inside!");
    e = criarEnemy();
    printf("\nAs well as all the crawly-crawlers making noise all over!\n\n");
    system("pause");
    system("cls");

    s    = criarPilha();
    printf("You take a step!");
    Sleep(2000);

    l    = criaLista();
    printf("\nAnd then another!\n\n");
    system("pause");
    system("cls");
    
    p    = criarPlayer();
    printf("You feel yourself filled with determination!\n\n");
    system("pause");
    system("cls");

    gameMenu(mapa, tam, s, l, p, e);
}

void restartGame(int** mapa, int tam, Player* p, Enemy* e, Pilha* s, Lista* l){
    mapa = criarMapa(p, tam);
    populaMapa(mapa, tam);
    e    = criarEnemy();
    s    = criarPilha();
    l    = criaLista();
    p    = criarPlayer();

    gameMenu(mapa, tam, s, l, p, e);
}

void gameOver(int** mapa, int tam, int end, Player* p, Enemy* e, Pilha* s, Lista* l){
    system("cls");
    switch(end){
        case 1:
            free(p);
            free(e);
            limparPilha(s);
            limparLista(l);

            printf("\tCongratulations!\n");
            printf("\nYou've beaten the dungeon!");
            limparMapa(mapa, tam);
            printf("\n\n*Crumbling noises");
            printf("\n\nAs you leave the dungeon, you hear it crumbling behind you\n\n");
            system("pause");
            break;
        
        case 0: 
            free(p);
            free(e);
            limparPilha(s);
            limparLista(l);
            
            printf("\tGAME OVER\n");
            printf("\nAfter arduos hours battling enemies and braving the dungeon,");
            printf("\nYou unfortunately succumb to you wounds");
            limparMapa(mapa, tam);
            printf("\nAnd the light slowly fades from your eyes\n\n");
            system("pause");
            break;
    }

    int opc;
    do{
        system("cls");
        printf("\nPress 1 to restart the game");
        printf("\nPress 0 to exit the game\n");
        scanf("%d", &opc);
        if     (opc == 1) restartGame(mapa, tam, p, e, s, l);
        else if(opc == 0) exit(1);
        else              {printf("\nInvalid choice"); opc = 10;}
    }while(opc == 10);
}

void gameMenu(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){    
    int choice = 0;;
    while(choice){
        system("cls");
        printMapa(mapa, tam);
        
        printf("\n\nWhat would you like to do?");
        scanf("%d", &choice);
        printf("\n1 - MOVE");
        printf("\t2 - OPEN BAG");
        printf("\t3 - GO BACK HOME");
        
        switch(choice){
            case 1: moveMenu(mapa, tam, s, l, p, e);    break;
            case 2: abrirInventario(l, p);              break;
            case 3: gameOver(mapa, tam, 2, p, e, s, l); break;
        }
    }
}

void moveMenu(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int move = 0;;
    while(move){
        system("cls");
        printMapa(mapa, tam);
        
        printf("\n\nWhere would you like to go?");
        printf("\n\t1 - Go Upwards");
        printf("\n2 - Go Leftwards");
        printf("\t\t3 - Go Rightwards");
        printf("\n\t4 - Go Downwards");
        printf("\n\n5 - Take a step backwards");
        
        switch(move){
            case 1: moverCima        (mapa, tam, s, l, p, e); break;
            case 2: moverEsquerda    (mapa, tam, s, l, p, e); break;
            case 3: moverDireita     (mapa, tam, s, l, p, e); break;
            case 4: moverBaixo       (mapa, tam, s, l, p, e); break;
            case 5: desfazerMovimento(mapa, tam, s, l, p, e); break;
        }
    }
}