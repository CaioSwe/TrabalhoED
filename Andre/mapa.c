#include "mapa.h"

int** criarMapa(int tam){
    int*** mapa = (int***)malloc(tam*sizeof(int**));
    srand(time(NULL));
    int input;

    int i_chance = rand() % (tam-2)+1;
    
    *mapa = (int**)malloc(tam*sizeof(int*));
    if(mapa == NULL){ printf("\nERROR: Insuficient Memory!"); exit(1); }

    for(int i = 0; i < tam; i++){
        (*mapa)[i] = (int*)malloc(tam*sizeof(int));
        if((*mapa) == NULL){ printf("\nERROR: Insuficient Memory!"); exit(1); }

        for(int j = 0; j < tam; j++){
            if     (i == 1 && j == 1)                             {(*mapa)[i][j] = 1; } // Representa o jogador
            else if(i == i_chance && j == tam-1)                  {(*mapa)[i][j] = 4; } // A saída da dungeon
            else if(i == 0 || i == tam-1 || j == 0 || j == tam-1) {(*mapa)[i][j] = 3; } // As bordas da dungeon 
            else                                                  {(*mapa)[i][j] = 0; } // Espaçoes em branco
        }        
    }
    
    // Preenche a dungeon com paredes
    (*mapa)[1][3] = 2;
    (*mapa)[2][3] = 2;
    (*mapa)[1][4] = 2;
    (*mapa)[2][4] = 2;
    (*mapa)[3][4] = 2;
    (*mapa)[4][4] = 2;
    (*mapa)[5][4] = 2;
    (*mapa)[6][4] = 2;
    (*mapa)[7][4] = 2;
    (*mapa)[1][5] = 2;
    (*mapa)[2][5] = 2;

    (*mapa)[10][7] = 2;
    (*mapa)[10][6] = 2;
    (*mapa)[10][5] = 2;
    (*mapa)[10][4] = 2;
    (*mapa)[10][3] = 2;
    (*mapa)[10][2] = 2;
    
    (*mapa)[5][1] = 2;
    (*mapa)[6][1] = 2;
    (*mapa)[7][1] = 2;
    (*mapa)[8][1] = 2;
    
    (*mapa)[3][8] = 2;
    (*mapa)[4][7] = 2;
    (*mapa)[4][8] = 2;
    (*mapa)[2][8] = 2;
    (*mapa)[2][9] = 2;
    (*mapa)[3][9] = 2;

    (*mapa)[9][6] = 2;
    (*mapa)[10][7] = 2;
    (*mapa)[9] [7] = 2;
    (*mapa)[8] [7] = 2;
    (*mapa)[7] [7] = 2;
    (*mapa)[8] [8] = 2;
    (*mapa)[9] [8] = 2;
    (*mapa)[7] [8] = 2;
    (*mapa)[10][8] = 2;
    (*mapa)[10][8] = 2;
    (*mapa)[10][9] = 2;

    (*mapa)[2][1] = 6;
    (*mapa)[3][1] = 6;
    (*mapa)[4][1] = 5;
    (*mapa)[4][2] = 6;
    (*mapa)[4][3] = 6;

    // printf("\nMap generated!\n");
    return *mapa;
}

void populaMapa(int** mapa, int tam){
    srand(time(NULL));
    int chance;

    // Atribui a chance de uma das casas ser o seguinte evento
    int trap    = 8;  // 8% = ~5 casas
    int enemies = 15; // 15% = ~10 casas
    int items   = 12; // 12% = ~7  casas

    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(mapa[i][j] == 0){
                chance = rand() % 100;
                if(chance < trap){
                    mapa[i][j] = 5;
                }
                else if(chance < items){
                    mapa[i][j] = 6;
                }
                else if(chance < enemies){
                    mapa[i][j] = 7;
                }
            }
        }
    }
}

void printMapa(int** mapa, int tam){
    printf("\n");

    // Imprime o mapa com os valores inteiros
    // for(int i = 0; i < tam; i++){
    //     for(int j = 0; j < tam; j++){
    //         printf("[%d]", mapa[i][j]);
    //     }printf("\n");
    // }printf("\n\n");

    // Imprime o mapa com os props gráficos
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if     (mapa[i][j] == 7) {printf(" ! ");} // Representa inimigos
            if     (mapa[i][j] == 6) {printf(" * ");} // Representa itens
            if     (mapa[i][j] == 5) {printf(" X ");} // Representa armadilhas
            if     (mapa[i][j] == 4) {printf(" O ");}
            else if(mapa[i][j] == 3) {printf(" # ");}
            else if(mapa[i][j] == 2) {printf(" # ");}
            else if(mapa[i][j] == 1) {printf(" @ ");}
            else if(mapa[i][j] == 0) {printf("   ");}
        }printf("\n");
    }printf("\n");
    Sleep(1000);
    system("cls");
}

void limparMapa(int** mapa, int tam){
    system("cls");
    printf("\nCongratulations!");
    printf("\nYou've beaten the dungeon!");

    for(int i = 0; i < tam; i++){
            free(mapa[i]);
    }free(mapa);
    printf("\n\n*Crumbling noises");
    
    printf("\n\nAs you leave the dungeon, you hear it crumbling behind you");
}

void ativarTrap(int** mapa, int tam, Player* p){
    int hp = getPlayerHP(p);
    printf("\n\nHP: %d", hp);

    printf("\nOh no!\nYou've stepped on a trap!");
    setPlayerHP(p, hp - 15);
    if(getPlayerHP(p) >= 0) gameOver(mapa, tam);

    printf("\nHP: %d\n\n", getPlayerHP(p));
}