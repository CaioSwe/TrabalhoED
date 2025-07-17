#include "Items.h"

Item* Item_Init(int id, ImageObject* spriteSheet){
    Item* item = (Item*)malloc(sizeof(Item));

    item->id = id;

    Rand(time(NULL));
    item->isMimic = (rand() % 100) < 30;

    item->sprite = spriteSheet;

    float x = 2.0f; // MUDAR

    if(item->sprite != NULL) item->sprite->source = (Rectangle){x*(item->id-1), 0.0f, 64.0f, 64.0f};

    return item;
}

Outcome usarItem(Item* item, Player* target){
    if(item->isMimic){
        return (Outcome){true, false};
    }

    Stats playerStats = Player_getStats(target);

    switch (item->id){
    case 1: // Health Potion
        printf("\nHP: %.2f", playerStats.health);

        int hp = playerStats.health + 40.0f;
        if(hp <= 100) Player_getHealing(target, hp);
        else          Player_getHealing(target, playerStats.health - playerStats.maxHealth);
        printf("\nHealth Potion used!");

        printf("\nHP: %.2f", Player_getStats(target));
        break;
    case 2: // Monster's Repelent
        Player_setRepelent(target, 10);
        printf("\nMonster's Repelent used!");
        break;
    default:
        printf("\nItem invalid.");
        Sleep(1500);
        break;
    }

    return (Outcome){false, true};
}

const char* getItemDescription(Item* item){
    switch (item->id){
    case 1: // Poção de vida
        return "It considerably heals you life energy!";
    case 2: // Repelente Monstro
        return "It makes it so your scent is undetectable to monsters.";
    default:
        return "Default Item Desc";
    }
}
