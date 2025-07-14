#include "inventario.h"

int itemChoice(){
    int input;
    printf("\n\nWould you like to collect it?");
    printf("\n1 - Yes");
    printf("\n2 - No\n");
    scanf("%d", &input);

    return input;
}

void coletarItem(int opc, Lista* l, Player* p, Enemy* e, Pilha* s, int** mapa, int tam){
    int input;
    do{
        switch(opc){
        // Health Potion
        case 1: 
            input = itemChoice();
            if(input == 1) inserirFim(l, 1);
            opc = 0;
            break;

        // Monster's Repelent
        case 2: 
            input = itemChoice();
            if(input == 1) inserirFim(l, 2);
            opc = 0;
            break;

        // Treasure Chest
        case 3: 
            int gold = rand() % 100 + 1;
            printf(" with %d coins of gold!", gold);
            input = itemChoice();
            if(input == 1){
                setPlayerGold(p, getPlayerGold(p)+gold);
                printf("\nYou've collected the coins");
                int mimic = rand() % 10;
                if(mimic <= 3){
                    system("cls");
                    printf("\nBut the chest was actually a mimic!"); Sleep(1500);
                    encounterEnemy(l, p, e, s, mapa, tam);
                }
            }
            Sleep(1500);
            system("cls");
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

void menuItem(Lista* l, Player* p, Enemy* e, Pilha* s, int** mapa, int tam){
    srand(time(NULL));
    int item = rand() % 3 + 1;

    system("cls");
    printf("You found an item!");

    // Escolhe qual item o usuário encontrou
    switch(item){ 
        case 1:
            printf("\nIt is a Health Potion!");
            coletarItem(1, l, p, e, s, mapa, tam);
            item = 0;
            break;
        case 2: 
            printf("\nIt is a Monster's Repelent!");
            coletarItem(2, l, p, e, s, mapa, tam);
            item = 0;
            break;
        case 3: 
            printf("\nIt is a Treasure chest");
            coletarItem(3, l, p, e, s, mapa, tam);
            item = 0;
            break;
        default: 
            printf("\nInvalid value ERROR");
            break;
    }
}

int menuInventario(){
    int opc;
    printf("\nWould you like to:");
    printf("\n1 - Use an item");
    printf("\n2 - Drop an item");
    printf("\n3 - Inspect an item"); 
    printf("\n4 - Exit the inventory\n"); 
    scanf("%d", &opc);

    return opc;
}

void abrirInventario(Lista* l, Player* p){
    system("cls");
    printf("You opened your inventory!");
    int opc = 0;
    do{
        printf("\nYou have the following items:\n\n");
        printf("GOLD: %d", getPlayerGold(p));
        imprimirLista(l);
        opc = menuInventario();
        switch(opc){
            case 1: 
                usarItem(l, p);
                system("cls");
                opc = 0;
                break;
            case 2: 
                descartarItem(l);
                system("cls");
                opc = 0;
                break;
            case 3:
                inspecionarItem(l);
                system("cls");
                opc = 0;
                break;
            case 4: // Fecha o inventário
                system("cls");
                opc = 4;
                break;
            default: 
                printf("\nSelected option unavailable >:(");
                Sleep(1500);
                system("cls");
                opc = 0;
                break;
        }
    }while(opc != 4);
}

void usarItem(Lista* l, Player* p){
    int opc;
    system("cls");
    printf("\nWhich item would you like to use?");
    imprimirLista(l);
    scanf("%d", &opc);

    Celula* item = buscarElemento(l, opc);
    if(item != NULL){
        switch (getValor(item)){

            // Health Potion
            case 1:
                printf("\nHP: %.2f", getPlayerHP(p));

                int hp = getPlayerHP(p) + 40;
                if(hp <= 100) setPlayerHP(p, hp);
                else          setPlayerHP(p, 100);
                printf("\nHealth Potion used!");

                printf("\nHP: %.2f", getPlayerHP(p));

                Sleep(1500);
                break;

            // Monster's Repelent
            case 2:
                setPlayerRepelent(p, 10);
                printf("\nMonster's Repelent used!");
                Sleep(1500);
                break;
        
            default:
                printf("\nInvalid choice");
                Sleep(1500);
                break;
        }removerMeio(l, opc);
    }else{
        printf("\nItem not found in inventory.");
    }
}

void descartarItem(Lista* l){
    int opc;
    system("cls");
    printf("\nWhich item would you like to drop?");
    imprimirLista(l);
    scanf("%d", &opc);
    removerMeio(l, opc);
}

void inspecionarItem(Lista* l){
    int opc;
    system("cls");
    printf("\nWhich item would you like to inspect?");
    imprimirLista(l);
    scanf("%d", &opc);

    Celula* item = buscarElemento(l, opc);

    if(item != NULL){
        switch (getValor(item)){

            // Health Potion
            case 1:
                printf("\nThis is a Health Potion");
                printf("\nIt considerably heals you life energy!");
                Sleep(3000);
                break;

            // Monster's Repelent
            case 2:
                printf("\nThis is a Monster's Repelent");
                printf("\nIt makes it so your scent is undetectable to monsters");
                printf(" for a short amount of time!");
                Sleep(3000);
                break;
            
            default:
                printf("\nInvalid choice");
                Sleep(1500);
                break;
        }
    }else printf("\nItem not found in inventory.");
}