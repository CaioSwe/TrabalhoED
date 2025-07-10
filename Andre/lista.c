#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct cel{
    int         valor;
    struct cel* prox;
}Celula;

struct lista{
    Celula* prim;
    Celula* ulti;
    int tamanho;
};

Lista* criaLista(){
    Lista* l = malloc(sizeof(Lista));

    l->prim  = NULL;
    l->ulti  = NULL;
    l->tamanho = 0;
    
    printf("\nLista criada!");
    return l;
}

int inserirInicio(Lista* lista, int v){
    Celula *nova = malloc(sizeof(Celula));
    nova->valor  = v;

    nova->prox  = lista->prim;
    lista->prim = nova;

    printf("\nCelula inserida no inicio!");
    lista->tamanho++;
    return 0;
}

int inserirMeio(Lista *lista, int v, int p){
    Celula *nova, *anterior;

    nova        = malloc(sizeof(Celula));
    nova->valor = v;

    if(lista->prim == NULL){ // A Lista está vazia
        nova->prox  = lista->prim;
        lista->prim = nova;
    }else{
        anterior = buscarElemento(lista, p);

        nova->prox = anterior->prox;
        anterior->prox = nova;
    }
    printf("\nCelula inserida no meio!");
    lista->tamanho++;
    return 0;
}

int inserirFim(Lista *lista, int v){
    Celula* aux = lista->prim;
    Celula *nova = malloc(sizeof(Celula));
    nova->valor  = v;

    if(lista->prim == NULL){ // A Lista está vazia
        nova->prox  = lista->prim;
        lista->prim = nova;
    }else{
        while(aux->prox != NULL)
            aux = aux->prox;
        nova->prox  = NULL;
        aux->prox   = nova;
        lista->ulti = nova;
    }

    lista->tamanho++;
    printf("\n\nYou've collected the item");
    Sleep(1500);
    system("cls");
    
    
    return 0;
}

int removerInicio(Lista* lista){
    Celula* del = lista->prim;
    int v = del->valor;

    lista->prim = lista->prim->prox;

    free(del);

    printf("\nCelula removida no inicio!");
    lista->tamanho--;
    return v;
}

int removerMeio(Lista* lista, int v){
    Celula* ant = NULL;
    Celula* del = lista->prim;

    while (del != NULL && del->valor != v){
        ant = del;
        del = del->prox;
    }

    if (del == NULL){
        printf("\nCelula nao encontrada");
        return 0;
    }

    ant->prox = del->prox;
    int conteudo = del->valor;
    free(del);
    
    printf("\nCelula removida no meio!");
    lista->tamanho--;
    return conteudo;
}

int removerFim(Lista* lista){
    Celula* ant = NULL;
    Celula* del = lista->prim;

    while(del->prox != NULL){
        ant = del;
        del = del->prox;
    }

    int v = del->valor;
    ant->prox = NULL;
    lista->ulti = ant->prox;

    printf("\nCelula removida no fim!");
    lista->tamanho--;
    return v;
}

Celula* buscarElemento(Lista* lista, int v){
    Celula* cel = lista->prim;

    while(cel != NULL){
        if(cel->valor == v)
                return cel;
        else
            cel = cel->prox;
    }

    return NULL; /* não achou o elemento */
}

void imprimirLista(Lista* lista){
    Celula* aux = lista->prim;

    printf("\n");
    int i = 1;
    while(aux != NULL){
        switch(aux->valor){
            case 1:  printf("%d - Health Potion\n", i);      i++; break;
            case 2:  printf("%d - Monster1s Repelent\n", i); i++; break;
            case 3:  printf("%d - Treasure Chest\n", i);     i++; break;
            default: printf("%d - ITEM ERROR\n", i);         i++; break;
        }
        aux = aux->prox;
    }
}

int getValor(Celula* cel){
    return cel->valor;
}