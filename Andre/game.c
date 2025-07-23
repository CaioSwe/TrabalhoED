#include "game.h"

void startGame(int** mapa, int tam, Player* p, Enemy* e, Pilha* s, Lista* l){

    /*
        Textos decorativos para introduzir o jogo :D
    */
    system("cls");
    printf("\t\tWelcome to the game!");
    printf("\n\n\t\t\tBy");
    printf("\nAndre Felipe Ijiri Ribeiro\tCaio Sweiwer de Carvalho");
    printf("\n\t\t\t:)\n\n");
    system("pause");
    system("cls");

    printf("\tOne day, you decide to go adventuring, despite you mother's worries");
    printf("\n'It's dangerous to go alone! Take this' She tells you, handing you an old but trusty sword");
    printf("\n\t\t\tAs well an old and dirty bag\n\n");
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

    s = criarPilha();
    printf("You take a step!");
    Sleep(1500);

    l = criaLista();
    printf("\nAnd then another!\n\n");
    system("pause");
    system("cls");
    
    p = criarPlayer();
    printf("You feel yourself filled with determination!\n\n");
    system("pause");
    system("cls");

    // Inicializa o jogo de fato
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

    /* EXECUTA O FINAL CORRETO DO JOGO */
    // Limpa e libera todas as estruturas e objetos
    free(p);
    free(e);
    limparPilha(s);
    limparLista(l);
    limparMapa(mapa, tam);
    /*  
        CASOS:
        0: Player morreu
        1: Ganhou o jogo
        2: Player desistiu
    */
    switch(end){
        case 1:
            printf("\tCongratulations!\n");
            printf("\nYou've beaten the dungeon!");
            printf("\n\n*Crumbling noises");
            printf("\n\nAs you leave the dungeon, you hear it crumbling behind you\n\n");
            system("pause");
            break;
        
        case 0: 
            printf("\tGAME OVER\n");
            printf("\nAfter arduos hours battling enemies and braving the dungeon,");
            printf("\nYou unfortunately succumb to you wounds");
            printf("\nAnd the light slowly fades from your eyes\n\n");
            system("pause");
            break;

        case 2: 
            printf("\tIt was simply too much\n");
            printf("\nIt seems the safety and comfort of your bed and mother's arms was much more attractive than your explorer spirit\n\n");
            system("pause");
            break;
    }

    // Recomeça ou encerra o jogo
    int opc;
    do{
        system("cls");
        printf("\nPress 1 to restart the game");
        printf("\nPress 0 to exit the game\n");
        scanf("%d", &opc);
        if     (opc == 1)  restartGame(mapa, tam, p, e, s, l);
        else if(opc == 0)  exit(1);
        else              {printf("\nInvalid choice"); opc = 10;}
    }while(opc == 10);
}

void gameMenu(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    system("cls");

    // Menu inicial básico do jogo (imprime o mapa e as escolhas de ações para o jogador)
    int choice;
    do{
        printMapa(mapa, tam);
        
        printf("\n\tWhat would you like to do?");
        printf("\n1 - MOVE");
        printf("\t2 - OPEN BAG");
        printf("\t3 - GO BACK HOME\n\n");
        scanf("%d", &choice);
        
        switch(choice){
            case 1: moveMenu(mapa, tam, s, l, p, e);    break;
            case 2: abrirInventario(l, p);              break;
            case 3: gameOver(mapa, tam, 2, p, e, s, l); break;
        }system("cls");

    }while(choice);
}

void moveMenu(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    system("cls");

    int choice = 0;
    printMapa(mapa, tam);
    
    printf("\n\n\tWhere would you like to go?\n");
    printf("\n\t\t8 - Go Upwards");                     /*              8 - Go Upwards                     */
    printf("\n4 - Go Leftwards");                       /*      4 - Go Leftwards    6 - Go Rightwards      */
    printf("\t\t6 - Go Rightwards");                    /*              2 - Go Downwards                   */
    printf("\n\t\t2 - Go Downwards");
    printf("\n\n\t\t1 - Take a step backwards\n\n");    /*         1 - Take a step backwards               */
    scanf("%d", &choice);
    
    switch(choice){
        case 8: 
            moverCima(mapa, tam, s, l, p, e); 
            printMapa(mapa, tam); 
            if(getPlayerRepelent(p) > 0) setPlayerRepelent(p, getPlayerRepelent(p)-1);
            break;

        case 4: 
            moverEsquerda(mapa, tam, s, l, p, e); 
            printMapa(mapa, tam); 
            if(getPlayerRepelent(p) > 0) setPlayerRepelent(p, getPlayerRepelent(p)-1);
            break;
        case 6: 
            moverDireita(mapa, tam, s, l, p, e); 
            printMapa(mapa, tam); 
            if(getPlayerRepelent(p) > 0) setPlayerRepelent(p, getPlayerRepelent(p)-1);
            break;

        case 2: 
            moverBaixo(mapa, tam, s, l, p, e); 
            printMapa(mapa, tam); 
            if(getPlayerRepelent(p) > 0) setPlayerRepelent(p, getPlayerRepelent(p)-1);
            break;

        case 1: desfazerMovimento(mapa, tam, s, l, p, e); printMapa(mapa, tam); break;
    }
}