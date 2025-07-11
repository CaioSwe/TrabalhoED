#include <stdbool.h>
#include "player.h"
typedef struct lista Lista;
typedef struct pilha Pilha;

bool moverCima         (int**, int, Pilha*, Lista*, Player*);
bool moverBaixo        (int**, int, Pilha*, Lista*, Player*);
bool moverDireita      (int**, int, Pilha*, Lista*, Player*);
bool moverEsquerda     (int**, int, Pilha*, Lista*, Player*);
void desfazerMovimento (int**, int, Pilha*                 );