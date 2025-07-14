#include "movimentacao.h"

bool moverCima(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int* pos = getPlayerPos(p);

    int i = pos[0] - 1;
    int j = pos[1];
    int move = mapa[i][j];
    switch(move){
        case 3: // Caminho bloqueado por parede
            printf("\nThe path upwards is blocked");
            printMapa(mapa, tam);

            return false;
            break;

        case 0: // Caminho está livre
            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
            printMapa(mapa, tam);
            break;

        case 4: // Saída da dungeon, fim do jogo
            limparMapa(mapa, tam);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
            printMapa(mapa, tam);
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
            
            printMapa(mapa, tam);
            break;

        case 7: // Encontrou um inimigo
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
            printMapa(mapa, tam);
            break;
    }
    return true;
}

bool moverBaixo(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int* pos = getPlayerPos(p);

    int i = pos[0] + 1;
    int j = pos[1];
    int move = mapa[i][j];
    switch(move){
        case 3: // Caminho bloqueado por parede
            printf("\nThe path downwards is blocked");
            printMapa(mapa, tam);

            return false;
            break;

        case 0: // Caminho está livre
            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
           
            printMapa(mapa, tam);
            break;

        case 4: // Saída da dungeon, fim do jogo
            limparMapa(mapa, tam);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
           
            printMapa(mapa, tam);
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
            
            printMapa(mapa, tam);
            break;

        case 7: // Encontrou um inimigo
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
           
            printMapa(mapa, tam);
            break;
    }
    return true;
}

bool moverEsquerda(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int* pos = getPlayerPos(p);

    int i = pos[0];
    int j = pos[1] - 1;
    int move = mapa[i][j];
    switch(move){
        case 3: // Caminho bloqueado por parede
            printf("\nThe path leftwards is blocked");
            printMapa(mapa, tam);

            return false;
            break;

        case 0: // Caminho está livre
            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
           
            printMapa(mapa, tam);
            break;

        case 4: // Saída da dungeon, fim do jogo
            limparMapa(mapa, tam);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p);

            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
           
            printMapa(mapa, tam);
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e);

            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
            
            printMapa(mapa, tam);
            break;

        case 7: // Encontrou um inimigo
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e);
      
            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
           
            printMapa(mapa, tam);
            break;
    }
    return true;
}

bool moverDireita(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int* pos = getPlayerPos(p);

    int i = pos[0];
    int j = pos[1] + 1;
    int move = mapa[i][j];
    switch(move){
        case 3: // Caminho bloqueado por parede
            printf("\nThe path rightwards is blocked");
            printMapa(mapa, tam);

            return false;
            break;

        case 0: // Caminho está livre
            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
           
            printMapa(mapa, tam);
            break;

        case 4: // Saída da dungeon, fim do jogo
            limparMapa(mapa, tam);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
           
            printMapa(mapa, tam);
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
            
            printMapa(mapa, tam);
            break;

        case 7: // Encontrou um inimigo
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
           
            printMapa(mapa, tam);
            break;
    }
    return true;
}

void desfazerMovimento(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int qntdMov = 0;
    int move = 0;
    do{
        system("cls");
        imprimirPilha(s);
        printf("\nHow many moves would you like to backtrack?\n");
        scanf("%d", &qntdMov);
        if(qntdMov > tamanhoPilha(s)){
            printf("\n\nYou've not walked that many steps yet, adventurer!");
            qntdMov = 0;
        }else{
            while(qntdMov != 0){
                move = pop(s);
                printf("Stack movement: %d", move);
                Sleep(1500);
                switch(move){
                    case 8: printf("\nMoving down") ; Sleep(1500); moverBaixo   (mapa, tam, s, l, p, e); pop(s); break;
                    case 2: printf("\nMoving up")   ; Sleep(1500); moverCima    (mapa, tam, s, l, p, e); pop(s); break;
                    case 4: printf("\nMoving right"); Sleep(1500); moverDireita (mapa, tam, s, l, p, e); pop(s); break;
                    case 6: printf("\nMoving left") ; Sleep(1500); moverEsquerda(mapa, tam, s, l, p, e); pop(s); break;                
                }
                qntdMov--;
            }
        }
    }while(qntdMov > tamanhoPilha(s));
}