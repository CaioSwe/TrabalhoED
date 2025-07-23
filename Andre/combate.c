#include "combate.h"

void encounterEnemy(Lista* l, Player* p, Enemy* e, Pilha* s, int** mapa, int tam){
    system("cls");

    srand(time(NULL));

    // Gera aleatóriamente a chance do baú ser um mímico
    if(getEnemyMimic(e) == true) printf("\nIt is a Mimic!\n");
    else{
        e = criarEnemy();
        int enemiesOpt = rand() % 2;
        printf("\tYou've encountere an enemy!");
        switch(enemiesOpt){
            case 0: printf("\nIt is a Skeleton!\n"); break;
            case 1: printf("\nIt is a Slime!\n"); break;
        }Sleep(3000);
    }
    
    // Menu de combate entre o jogador e o inimigo
    int opc = 0;
    int escape = 0;
    do{
        system("cls");

        printf("ENEMY:\t%.2f", getEnemyHP(e));
        printf("\nPLAYER:\t%.2f", getPlayerHP(p));

        printf("\n\nIt is your turn, would you like to:");
        printf("\n1 - ATTACK");
        printf("\t2 - DEFEND");
        printf("\t3 - USE ITEM");
        printf("\t4 - ESCAPE\n");
        scanf("%d", &opc);
    
        /*
            CASOS:
            1: Atacar
            2: Defender
            3: Usar item
            4: Tentar escapar
        */
        switch(opc){
            case 1:
                attackEnemy(p, e);
                if(getEnemyHP(e) > 0) enemyTurn(p, e, s, l, mapa, tam);
                break;
    
            case 2:
                defendFromEnemy(p, e, s, l, mapa, tam);
                if(getEnemyHP(e) > 0) enemyTurn(p, e, s, l, mapa, tam);
                break;
    
            case 3:
                usarItem(l, p);
                enemyTurn(p, e, s, l, mapa, tam);
                break;
    
            case 4:
                escape = tryEscape(p); Sleep(3000);
                if(escape == 0) enemyTurn(p, e, s, l, mapa, tam);
                break;
        }
    }while(getPlayerHP(p) > 0 && getEnemyHP(e) > 0 && escape == 0);
    criarEnemy();
}

void attackEnemy(Player* p, Enemy* e){
    system("cls");

    float attack = getPlayerAttack(p) - getPlayerAttack(p)*getEnemyDefense(e);

    printf("ENEMY:\t%.2f", getEnemyHP(e));
    printf("\nPLAYER:\t%.2f", getPlayerHP(p));

    printf("\n\nYou swing you sword, damaging the enemy!");
    setEnemyHP(e, getEnemyHP(e) - attack);
    int enemyDead = isEnemyDead(p, e);

    if(enemyDead == 1) return;
    else{
        printf("\n'AAAARGH' It screams in pain");
        printf("\n-%.2f", attack);
    
        printf("\n\nENEMY:\t%.2f", getEnemyHP(e));
        printf("\nPLAYER:\t%.2f\n\n", getPlayerHP(p));

        system("pause");
    }
}

void defendFromEnemy(Player* p, Enemy* e, Pilha* s, Lista* l, int** mapa, int tam){
    system("cls");

    float attack = getEnemyAttack(e) - getEnemyAttack(e)*getPlayerDefense(p);

    printf("ENEMY:\t%.2f", getEnemyHP(e));
    printf("\nPLAYER:\t%.2f", getPlayerHP(p));

    printf("\n\nYou rise your shield, defending yourself");
    float chance = rand() % 100 + 1;
    if(chance > getPlayerDodge(p)){
        printf("\n-%.2f", attack);
        setPlayerHP(p, getPlayerHP(p) - attack);    
        int playerDead = isPlayerDead(p, e, s, l, mapa, tam);
    }else printf("\nBut you manage to dodge it's attack!");

    printf("\n\nRecovering from it's attack, you manage to find a small opening and nick it");
    float dmg = (getPlayerAttack(p) - getPlayerAttack(p)*getEnemyDefense(e))/2;
    printf("\n-%.2f", dmg);
    setEnemyHP(e, getEnemyHP(e) - dmg);

    int enemyDead = isEnemyDead(p, e);
    if(enemyDead == 1) return;
    else{
        printf("\n'AAAARGH' It screams in pain");
        printf("\n-%.2f", attack);
    
        printf("\n\nENEMY:\t%.2f", getEnemyHP(e));
        printf("\nPLAYER:\t%.2f\n\n", getPlayerHP(p));

        system("pause");
    }
}

bool tryEscape(Player* p){
    system("cls");

    printf("You've tried to escape the encounter");
    float chance = rand() % 100 + 1;
    if(chance <= getPlayerEvasionRate(p)){
        printf(" and succeded!");
        return 1;
    }
    else{
        printf(" but failed");
        return 0;
    }
}

void enemyTurn(Player* p, Enemy* e, Pilha* s, Lista* l, int** mapa, int tam){
    system("cls");

    int playerDead;
    float attack = getEnemyAttack(e) - getEnemyAttack(e)*getPlayerDefense(p);

    printf("ENEMY:\t%.2f", getEnemyHP(e));
    printf("\nPLAYER:\t%.2f", getPlayerHP(p));

    printf("\n\nIt is the enemy's turn, he ");

    /*                  
                CONDICIONAL
            SIM             NÃO
    ____________________________________________
    ____________________________________________

                HPInimigo <= 1/2
        Cura HP                 HPJogador <= 1/4
                           Ataca                HPJogador <= 1/2
                                   Ataca/Defende                Ataca
    */
    if(getEnemyHP(e) <= getEnemyHP(e)/2){
        int chance = rand() % 100 + 1;
        if(chance <= 10) { setEnemyHP(e, getEnemyHP(e)+20); printf(" recovered +20 HP"); }
        else               printf(" tried to recover, but failed");

    }else if(getPlayerHP(p) <= getPlayerHP(p)*0.25){
        setPlayerHP(p, getPlayerHP(p) - attack);
        printf(" attacked you");
        printf("\n-%.2f", attack);

        playerDead = isPlayerDead(p, e, s, l, mapa, tam);

    }else if(getPlayerHP(p) < getPlayerHP(p)/2){
        int chance = rand() % 1;
        if(chance == 1){ 
            setPlayerHP(p, getPlayerHP(p) - attack); 
            printf(" attacked you");
            printf("\n-%.2f", attack);

            playerDead = isPlayerDead(p, e, s, l, mapa, tam);

        }else{
            setEnemyHP(e, getEnemyHP(e) - getPlayerAttack(p)-getPlayerAttack(p)*getEnemyDefense(e));
            printf(" defended against yout attack");
            printf("\n-%.2f", getPlayerAttack(p)-getPlayerAttack(p)*getEnemyDefense(e));
        }
    }else{
        setPlayerHP(p, getPlayerHP(p) - attack); 
        printf(" attacked you");
        printf("\n-%.2f", attack);

        playerDead = isPlayerDead(p, e, s, l, mapa, tam);
    }

    printf("\n\nENEMY:\t%.2f", getEnemyHP(e));
    printf("\nPLAYER:\t%.2f\n\n", getPlayerHP(p));

    system("pause");
}

bool isEnemyDead(Player* p, Enemy* e){
    if(getEnemyHP(e) <= 0){
        free(e);
        printf("\nIt proved enough and it falls dead on the dungeon floor!");
        printf("\n\nENEMY:\t PERISHED");
        printf("\nPLAYER:\t%.2f\n\n", getPlayerHP(p));
        
        system("pause");
        return 1;
    }else return 0;
}

bool isPlayerDead(Player* p, Enemy* e, Pilha* s, Lista* l, int** mapa, int tam){
    if(getPlayerHP(p) <= 0){
        free(p);
        printf("\nSadly, it wasn't enough and it's attack goes through your shield, killing you");
        printf("\n\nENEMY:\t%.2f", getEnemyHP(e));
        printf("\nPLAYER:\tPERISHED\n\n");

        system("pause");
        gameOver(mapa, tam, 0, p, e, s, l);
    }else return 0;
}