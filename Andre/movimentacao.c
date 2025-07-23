#include "movimentacao.h"

bool moverCima(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int* pos = getPlayerPos(p);

    int i = pos[0] - 1;
    int j = pos[1];
    int move = mapa[i][j];
    switch(move){
        /*
            CASOS:
            0: Espaço livre
            3: Parede
            4: Saída da dungeon
            5: Armadilha
            6: Item
            7: Inimigo
        */
        case 3:
            printf("\nThe path upwards is blocked");
            return false;
            break;

        case 0:
            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
            break;

        case 4:
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5:
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
            break;
        
        case 6:
            menuItem(l, p, e, s, mapa, tam);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
            
            break;

        case 7:
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e, s, mapa, tam);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
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
        /*
            CASOS:
            0: Espaço livre
            3: Parede
            4: Saída da dungeon
            5: Armadilha
            6: Item
            7: Inimigo
        */
        case 3:
            printf("\nThe path downwards is blocked");
            return false;
            break;

        case 0:
            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
                       
            break;

        case 4:
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5:
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
                       
            break;
        
        case 6:
            menuItem(l, p, e, s, mapa, tam);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
                        
            break;

        case 7:
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e, s, mapa, tam);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
                       
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
        /*
            CASOS:
            0: Espaço livre
            3: Parede
            4: Saída da dungeon
            5: Armadilha
            6: Item
            7: Inimigo
        */
        case 3:
            printf("\nThe path leftwards is blocked");
            return false;
            break;

        case 0:
            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
                       
            break;

        case 4:
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5:
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
                       
            break;
        
        case 6:
            menuItem(l, p, e, s, mapa, tam);

            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
                        
            break;

        case 7:
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e, s, mapa, tam);
      
            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
                       
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
        /*
            CASOS:
            0: Espaço livre
            3: Parede
            4: Saída da dungeon
            5: Armadilha
            6: Item
            7: Inimigo
        */
        case 3:
            printf("\nThe path rightwards is blocked");
            return false;
            break;

        case 0:
            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
                       
            break;

        case 4:
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5:
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
           
            break;
        
        case 6:
            menuItem(l, p, e, s, mapa, tam);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
                        
            break;

        case 7:
            if(getPlayerRepelent(p) <= 0) encounterEnemy(l, p, e, s, mapa, tam);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
                       
            break;
    }
    return true;
}

void desfazerMovimento(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    system("cls");

    int move = 0;
    move = pop(s);
    switch(move){
        case 8: moverBaixo   (mapa, tam, s, l, p, e); pop(s); break;
        case 2: moverCima    (mapa, tam, s, l, p, e); pop(s); break;
        case 4: moverDireita (mapa, tam, s, l, p, e); pop(s); break;
        case 6: moverEsquerda(mapa, tam, s, l, p, e); pop(s); break;                
    }
}