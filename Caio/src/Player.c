#include "Player.h"

Player* Player_Init(Rectangle source, Rectangle destination, const char* spriteSheet){
    Player* player = (Player*)malloc(sizeof(Player));

    player->anim = (Animation*)malloc(sizeof(Animation));
    player->anim->position = (PositionAnimation*)malloc(sizeof(PositionAnimation));
    player->anim->scale = (ScaleAnimation*)malloc(sizeof(ScaleAnimation));
    player->spriteSheet = LoadTexture(spriteSheet);
    
    player->anim->position->function = linearFunction;

    player->anim->frames = (FramesAnimation){true, 10.0f, 0.0f, 1, 0, 3, 0, 6};

    player->display = (Vector2){0, 120.0f};
    player->deltaX = 160.0f;

    player->source = source;
    player->destination = destination;

    player->stepDistance = 1.0f;
    player->characterChoice = false;

    player->stats = (Stats){10.0f, 10.0f, 2.0f, 0.05f, 0.1f, false, 0}; // Vida máxima, Vida, Ataque, Defesa, Evasão (Base), Ouro

    player->inventario = (Inventario){criaLista(), criaLista()};

    return player;
}

Vector2 Player_GetCoords(Player* player){
    return player->coordinates;
}

void Player_SetStepSize(Player* player, float stepSize){
    player->stepDistance = stepSize;
}

void Player_Update(Player* player, float deltaTime){
    Player_UpdatePosition(player, deltaTime);
    Player_UpdateSprite(player, true, true);

    player->coordinates.x = player->destination.x / player->stepDistance;
    player->coordinates.y = player->destination.y / player->stepDistance;
}

void Player_SetMoveSet(Player* player, MoveSet moveSet){
    player->keys = moveSet;
}

void Player_UpdatePosition(Player* player, float deltaTime){
    if(!player->anim->position->animating){
        if(IsKeyDown(player->keys.up)) Player_StepTo(player, (Vector2){0,-1}, true);
        if(IsKeyDown(player->keys.down)) Player_StepTo(player, (Vector2){0,1}, true);
        if(IsKeyDown(player->keys.left)) Player_StepTo(player, (Vector2){-1,0}, true);
        if(IsKeyDown(player->keys.right)) Player_StepTo(player, (Vector2){1,0}, true);
    }

    Vector2 current = UpdateItemPosition(player->anim->position, deltaTime);

    player->destination.x = current.x;
    player->destination.y = current.y;
}

void Player_Resize(Player* player, float ratio, float interval){
    ResizeItem(player->anim->scale, ratio, interval);
}

void Player_UpdateSize(Player* player, float deltaTime){
    player->destination = UpdateItemSize(player->anim->scale, player->destination, deltaTime);
}

void Player_ChangeCharacter(Player* player){
    player->characterChoice = !player->characterChoice;
}

void Player_UpdateSprite(Player* player, bool useKeys, bool reverse){
    player->anim->frames.framesCounter++;

    if (player->anim->frames.framesCounter >= (60/player->anim->frames.framesSpeed) && player->anim->frames.animating){
        player->anim->frames.framesCounter = 0;
        player->anim->frames.currentFrame += player->anim->frames.animationDirection;

        if(reverse){
            if (player->anim->frames.currentFrame > player->anim->frames.amountOfFrames - 1){
                player->anim->frames.animationDirection = -1;
                player->anim->frames.currentFrame = player->anim->frames.amountOfFrames - 2;
            }
            if (player->anim->frames.currentFrame < 0){
                player->anim->frames.animationDirection = 1;
                player->anim->frames.currentFrame = 1;
            }
        }
        else{
            if(player->anim->frames.currentFrame > player->anim->frames.amountOfFrames - 1){
                player->anim->frames.currentFrame = 0;
            }
        }

        if (player->anim->frames.currentFrame != 0) player->anim->frames.padding += player->anim->frames.framesPadding * player->anim->frames.animationDirection;
        else player->anim->frames.padding = 0.0f;

        player->display.x = (float)player->anim->frames.currentFrame*player->deltaX + player->anim->frames.padding;
    }

    if(!player->anim->frames.animating){
        player->anim->frames.framesCounter = 0;
        player->anim->frames.currentFrame = 1;
        player->display.x = (float)player->anim->frames.currentFrame*160.0f + 10.0f;
        player->anim->frames.animationDirection = -1;
    }

    int character = (player->characterChoice) ? 2020.0f : 0.0f;

    if(!player->anim->position->animating && useKeys){
        if(IsKeyDown(player->keys.up) || player->moving.up) player->display.y = 450.0f;
        if(IsKeyDown(player->keys.down) || player->moving.down) player->display.y = 120.0f;
        if(IsKeyDown(player->keys.left) || player->moving.left) player->display.y = 780.0f;
        if(IsKeyDown(player->keys.right) || player->moving.right) player->display.y = 1110.0f;
    }

    bool allKeys = (IsKeyDown(player->keys.up) || IsKeyDown(player->keys.down) || IsKeyDown(player->keys.left) || IsKeyDown(player->keys.right));

    player->anim->frames.animating = (allKeys || player->anim->position->animating) ? true : false;

    player->source.x = player->display.x;
    player->source.y = player->display.y + character;
}

void Player_StepTo(Player* player, Vector2 direction, bool updateSprite){
    if(abs((int)direction.x) != abs((int)direction.y)){
        Vector2 finalPoint = (Vector2){player->destination.x + player->stepDistance * direction.x, player->destination.y + player->stepDistance * direction.y};
        Vector2 startPoint = (Vector2){player->destination.x, player->destination.y};

        player->moving.up = (direction.y == -1) ? true : false;
        player->moving.down = (direction.y == 1) ? true : false;
        player->moving.left = (direction.x == -1) ? true : false;
        player->moving.right = (direction.x == 1) ? true : false;

        if(updateSprite) Player_UpdateSprite(player, true, true);

        MoveItemTo(player->anim->position, startPoint, finalPoint, 0.3f);
    }
}

void Player_MoveTo(Player* player, Vector2 newPos, float interval){
    MoveItemTo(player->anim->position, (Vector2){player->destination.x, player->destination.y}, newPos, interval);
}

void Player_SetSpriteSheet(Player* player, const char* spriteSheet){
    player->spriteSheet = LoadTexture(spriteSheet);
}

void Player_SetSourceRec(Player* player, Rectangle newSource){
    player->source = newSource;
    player->display = (Vector2){newSource.x, newSource.y};
}

void Player_SetDestRec(Player* player, Rectangle newDest){
    player->destination = newDest;
}

void Player_SetSpriteSize(Player* player, int width, int height){
    player->destination.width = width;
    player->destination.height = height;
}

void Player_Draw(Player* player){
    DrawTexturePro(player->spriteSheet, player->source, player->destination, (Vector2){0,0}, 0.0f, WHITE);
}

/////////////////////////////////////////////////////////////////////////

bool isLife50(Player* player){
    (player->stats.health >= 50) ? printf("\n%.0f >= 50 ? true", player->stats.health) : printf("\n%.0f >= 50 ? false", player->stats.health);

    return (player->stats.health >= 50);
}

bool isLife30(Player* player){
    (player->stats.health >= 30) ? printf("\n%.0f >= 30 ? true", player->stats.health) : printf("\n%.0f >= 30 ? false", player->stats.health);

    return (player->stats.health >= 30);
}

//////////////////////////////////////////////////////////////////////////

void Player_TakeDamage(Player* player, float damage){
    player->stats.evasionRate = (player->stats.defending) ? player->stats.evasionRate + 0.05f : player->stats.evasionRate + (damage / player->stats.health);
    player->stats.health = (player->stats.defending) ? player->stats.health - ((1 - player->stats.defense) * damage) : player->stats.health - damage;

    printf("\nPlayer took %.1f damage.", damage);
}

void Player_UpdateAtk(Player* player, float newAtk){
    player->stats.attack = newAtk;
}

void Player_UpdateDef(Player* player, float newDef){
    player->stats.defense = newDef;
}

void Player_getHealing(Player* player){
    player->stats.health += 20;
}

void Player_Print(Player* player){
    printf("\nPlayer healed 20hp");
    player->stats.health += 2.0f;
}