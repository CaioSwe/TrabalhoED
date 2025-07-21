#include "movimentacao.h"

bool moverCima(int** mapa, int tam, Pilha* s, Lista* l, Player* p, Enemy* e){
    int* pos = getPlayerPos(p);

    int i = pos[0] - 1;
    int j = pos[1];
    int move = mapa[i][j];
    switch(move){
        case 3: // Caminho bloqueado por parede
            printf("\nThe path upwards is blocked");
            return false;
            break;

        case 0: // Caminho está livre
            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
            break;

        case 4: // Saída da dungeon, fim do jogo
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
           
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e, s, mapa, tam);

            mapa[ i ][j] = 1;
            mapa[i+1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 8);
            
            break;

        case 7: // Encontrou um inimigo
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
        case 3: // Caminho bloqueado por parede
            printf("\nThe path downwards is blocked");
            return false;
            break;

        case 0: // Caminho está livre
            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
                       
            break;

        case 4: // Saída da dungeon, fim do jogo
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
                       
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e, s, mapa, tam);

            mapa[ i ][j] = 1;
            mapa[i-1][j] = 0;
            setPlayerPos(p, i, j);
            push(s, 2);
                        
            break;

        case 7: // Encontrou um inimigo
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
        case 3: // Caminho bloqueado por parede
            printf("\nThe path leftwards is blocked");
            return false;
            break;

        case 0: // Caminho está livre
            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
                       
            break;

        case 4: // Saída da dungeon, fim do jogo
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
                       
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e, s, mapa, tam);

            mapa[i][ j ] = 1;
            mapa[i][j+1] = 0;
            setPlayerPos(p, i, j);
            push(s, 4);
                        
            break;

        case 7: // Encontrou um inimigo
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
        case 3: // Caminho bloqueado por parede
            printf("\nThe path rightwards is blocked");
            return false;
            break;

        case 0: // Caminho está livre
            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
                       
            break;

        case 4: // Saída da dungeon, fim do jogo
            gameOver(mapa, tam, 1, p, e, s, l);
            break;

        case 5: // Pisou em uma armadilha
            ativarTrap(mapa, tam, p, e, s, l);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
           
            break;
        
        case 6: // Passou por cima de um item
            menuItem(l, p, e, s, mapa, tam);

            mapa[i][ j ] = 1;
            mapa[i][j-1] = 0;
            setPlayerPos(p, i, j);
            push(s, 6);
                        
            break;

        case 7: // Encontrou um inimigo
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