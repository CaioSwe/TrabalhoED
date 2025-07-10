#include "inventario.h"
#include "lista.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int itemChoice(){
    int input;
    printf("\n\nWould you like to collect it?");
    printf("\n1 - Yes");
    printf("\n2 - No\n");
    scanf("%d", &input);

    return input;
}

void coletarItem(int opc, Lista* l){
    int input;
    do{
        switch(opc){
        case 1: 
            input = itemChoice();
            if(input == 1) {inserirFim(l, 1);}
            opc = 0;
            break;
        case 2: 
            input = itemChoice();
            if(input == 1) {inserirFim(l, 2);}
            opc = 0;
            break;
        case 3: 
            input = itemChoice();
            if(input == 1) {inserirFim(l, 3);}
            opc = 0;
            break;
        default:
            printf("\nSelected option unavailable >:(");
            Sleep(1500);
            system("cls");
            break;
        }
    }while(opc != 0);

}

void menuItem(Lista* l){
    srand(time(NULL));
    int item = rand() % 3 + 1;
    int gold = rand() % 100 + 1;

    system("cls");
    printf("You found an item!");

    do{
        switch(item){ // Escolhe qual item o usuário encontrou
            case 1:
                printf("\nIt is a Health Potion!");
                coletarItem(1, l);
                item = 0;
                break;
            case 2: 
                printf("\nIt is a Monster's Repelent!");
                coletarItem(2, l);
                item = 0;
                break;
            case 3: 
                printf("\nIt is a Treasure chest with %d coins of gold!", gold);
                coletarItem(3, l);
                item = 0;
                break;
            default: 
                printf("\nInvalid value ERROR");
                break;
        }
    }while(item != 0);
}

int menuInventario(){
    int opc;
    printf("\n\n");
    printf("Would you like to:");
    printf("\n1 - Use an item");
    printf("\n2 - Drop an item");
    printf("\n3 - Inspect an item\n"); 
    scanf("%d", &opc);

    return opc;
}

void abrirInventario(Lista* l, Player* p){
    system("cls");
    printf("\n\nYou opened your inventory!");
    printf("\nYou have the following items:\n");
    imprimirLista(l);
    int opc = menuInventario();

    do{
        switch(opc){
            case 1: 
                usarItem(l, p);
                opc = 0;
                break;
            case 2: 
                //descartarItem();
                opc = 0;
                break;
            case 3:
                // printf("");
                // inspecionarItem();
                opc = 0;
                break;
            default: 
                printf("\nSelected option unavailable >:(");
                Sleep(1500);
                system("cls");
                break;
        }
    }while(opc != 0);
}

void usarItem(Lista* l, Player* p){
    int opc;
    printf("\n\nWhich item would you like to use?");
    imprimirLista(l);
    scanf("%d", &opc);

    Celula* item = buscarElemento(l, opc);
    if(item != NULL){
        switch (getValor(item)){
            case 1:
                
                break;
        
            default:
                break;
        }
        printf("\nYou used the item: %d", getValor(item));
        removerMeio(l, opc);
    }else{
        printf("\nItem not found in inventory.");
    }
}