#include "Arvore.h"

Arvore* criaFolha(bool (*funcao)(const void*), void* target){
    Arvore* arvore = (Arvore*)malloc(sizeof(Arvore));
    arvore->esq = NULL;
    arvore->dir = NULL;
    arvore->funcao = funcao;
    arvore->target = target;

    return arvore;
}

void inserirEsqArvore(Arvore* raiz, bool (*funcao)(const void*), void* target){
    raiz->esq = criaFolha(funcao, target);
}

void inserirDirArvore(Arvore* raiz, bool (*funcao)(const void*), void* target){
    raiz->dir = criaFolha(funcao, target);
}

void percorrerArvore(Arvore* raiz){
    if((raiz->esq == NULL && raiz->dir == NULL) || raiz == NULL){
        return;
    }

    if(raiz->funcao(raiz->target)){
        printf("\n | Indo para a esquerda -> ");
        percorrerArvore(raiz->esq);
    }
    else{
        printf("\n | Indo para a direita -> ");
        percorrerArvore(raiz->dir);
    }

    return;
}