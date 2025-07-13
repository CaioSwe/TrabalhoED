#include "movimentacao.h"

bool moverCima(int** mapa, int tam, Pilha* s, Lista* l, Player* p){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){            
            if(mapa[i][j] == 1){ // Encontra o jogador no mapa
                
                // ACABOU O JOGO
                if(mapa[i-1][j] == 4){
                    limparMapa(mapa, tam);

                    mapa[i-1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 8);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i-1][j] == 5){
                    ativarTrap(mapa, tam, p);

                    mapa[i-1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 8);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i-1][j] == 6){
                    menuItem(l, p);

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
                    limparMapa(mapa, tam);

                    mapa[i+1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 2);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i+1][j] == 5){
                    ativarTrap(mapa, tam, p);
                    
                    mapa[i+1][j] = 1;
                    mapa[i][j] = 0;
                    push(s, 2);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i+1][j] == 6){
                    menuItem(l, p);

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
                    limparMapa(mapa, tam);

                    mapa[1][j-1] = 1;
                    mapa[i][j] = 0;
                    push(s, 4);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i][j-1] == 5){
                    ativarTrap(mapa, tam, p);
                    
                    mapa[i][j-1] = 1;
                    mapa[i][j] = 0;
                    push(s, 4);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i][j-1] == 6){
                    menuItem(l, p);

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
                    limparMapa(mapa, tam);

                    mapa[1][j+1] = 1;
                    mapa[i][j] = 0;
                    push(s, 6);
                    printMapa(mapa, tam);
                }
                
                // ARMADILHAS
                else if(mapa[i][j+1] == 5){
                    ativarTrap(mapa, tam, p);
                    
                    mapa[i][j+1] = 1;
                    mapa[i][j] = 0;
                    push(s, 6);
                    printMapa(mapa, tam);
                }
                
                // ITENS
                else if(mapa[i][j+1] == 6){
                    menuItem(l, p);

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

void desfazerMovimento(int** mapa, int tam, Pilha* s, Lista* l, Player* p){
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
                    case 8: printf("\nMoving down") ; Sleep(1500); moverBaixo   (mapa, tam, s, l, p); pop(s); break;
                    case 2: printf("\nMoving up")   ; Sleep(1500); moverCima    (mapa, tam, s, l, p); pop(s); break;
                    case 4: printf("\nMoving right"); Sleep(1500); moverDireita (mapa, tam, s, l, p); pop(s); break;
                    case 6: printf("\nMoving left") ; Sleep(1500); moverEsquerda(mapa, tam, s, l, p); pop(s); break;                
                }
                qntdMov--;
            }
        }
    }while(qntdMov > tamanhoPilha(s));
}