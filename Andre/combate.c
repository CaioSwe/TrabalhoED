#include "combate.h"

void encounterEnemy(Lista* l, Player* p, Enemy* e){
    system("cls");
    srand(time(NULL));
    e = criarEnemy();

    int enemiesOpt = rand() % 3;
    printf("\tYou've encountere an enemy!");
    switch(enemiesOpt){
        case 0:
        printf("\nIt is a X\n");
        break;
        case 1:
        printf("\nIt is a Y\n");
        break;
        case 2:
        printf("\nIt is a Z\n");
        break;
    }Sleep(2500);
    
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
    
        switch(opc){
            case 1:
                attackEnemy(p, e);
                if(getEnemyHP(e) > 0) enemyTurn(p, e);
                break;
    
            case 2:
                defendFromEnemy(p, e);
                if(getEnemyHP(e) > 0) enemyTurn(p, e);
                break;
    
            case 3:
                usarItem(l, p);
                enemyTurn(p, e);
                break;
    
            case 4:
                escape = tryEscape(); Sleep(2000);
                if(escape == 0) enemyTurn(p, e);
                break;
        }
    }while(getPlayerHP(p) > 0 && getEnemyHP(e) > 0 && escape == 0);
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
        printf("\nPLAYER:\t%.2f", getPlayerHP(p));
        Sleep(5000);
    }
}

void defendFromEnemy(Player* p, Enemy* e){
    system("cls");

    float attack = getEnemyAttack(e) - getEnemyAttack(e)*getPlayerDefense(p);

    printf("ENEMY:\t%.2f", getEnemyHP(e));
    printf("\nPLAYER:\t%.2f", getPlayerHP(p));

    printf("\n\nYou rise your shield, defending yourself");
    printf("\n-%.2f", attack);
    setPlayerHP(p, getPlayerHP(p) - attack);

    int playerDead = isPlayerDead(p, e);

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
        printf("\nPLAYER:\t%.2f", getPlayerHP(p));
        Sleep(5000);
    }
}

bool tryEscape(){
    system("cls");

    printf("You've tried to escape the encounter");
    int chance = rand() % 100 + 1;
    if(chance <= 10){
        printf(" and succeded!");
        return 1;
    }
    else{
        printf(" but failed");
        return 0;
    }
}

void enemyTurn(Player* p, Enemy* e){
    system("cls");

    int playerDead;
    float attack = getEnemyAttack(e) - getEnemyAttack(e)*getPlayerDefense(p);

    printf("ENEMY:\t%.2f", getEnemyHP(e));
    printf("\nPLAYER:\t%.2f", getPlayerHP(p));

    printf("\n\nIt is the enemy's turn, he ");

    if(getEnemyHP(e) <= getEnemyHP(e)/2){
        int chance = rand() % 100 + 1;
        if(chance <= 10) { setEnemyHP(e, getEnemyHP(e)+20); printf(" recovered +20 HP"); }
        else               printf(" tried to recover, but failed");

    }else if(getPlayerHP(p) < getPlayerHP(p)*0.25){
        setPlayerHP(p, getPlayerHP(p) - attack);
        printf(" attacked you");
        printf("\n-%.2f", attack);

        playerDead = isPlayerDead(p, e);

    }else if(getPlayerHP(p) < getPlayerHP(p)/2){
        int chance = rand() % 1;
        if(chance == 1){ 
            setPlayerHP(p, getPlayerHP(p) - attack); 
            printf(" attacked you");
            printf("\n-%.2f", attack);

            playerDead = isPlayerDead(p, e);

        }else{
            setEnemyHP(e, getEnemyHP(e) - getPlayerAttack(p)-getPlayerAttack(p)*getEnemyDefense(e));
            printf(" defended against yout attack");
            printf("\n-%.2f", getPlayerAttack(p)-getPlayerAttack(p)*getEnemyDefense(e));
        }
    }else{
        setPlayerHP(p, getPlayerHP(p) - attack); 
        printf(" attacked you");
        printf("\n-%.2f", attack);

        playerDead = isPlayerDead(p, e);
    }

    printf("\n\nENEMY:\t%.2f", getEnemyHP(e));
    printf("\nPLAYER:\t%.2f", getPlayerHP(p));

    Sleep(5000);
}

bool isEnemyDead(Player* p, Enemy* e){
    if(getEnemyHP(e) <= 0){
        free(e);
        printf("\nIt proved enough and it falls dead on the dungeon floor!");
        printf("\n\nENEMY:\t PERISHED");
        printf("\nPLAYER:\t%.2f", getPlayerHP(p));
        Sleep(5000);
        return 1;
    }else return 0;
}

bool isPlayerDead(Player* p, Enemy* e){
    if(getPlayerHP(p) <= 0){
        free(p);
        printf("\nSadly, it wasn't enough and it's attack goes through your shield, killing you");
        printf("\n\nENEMY:\t%.2f", getEnemyHP(e));
        printf("\nPLAYER:\tPERISHED");
        Sleep(5000);
        // GAMEOVER();
    }else return 0;
}