#ifndef _Player_h_
#define _Player_h_

#include "ImageObject.h"
#include "Utils.h"
#include "Delimiters.h"
#include "Lista.h"
#include "Animation.h"

typedef struct MoveSet{
    int up;
    int down;
    int left;
    int right;
} MoveSet;

typedef struct Moving{
    bool up;
    bool down;
    bool left;
    bool right;
} Moving;

typedef struct Animation{
    PositionAnimation* position;
    ScaleAnimation* scale;
    FramesAnimation frames;
} Animation;

typedef struct Stats{
    float maxHealth;
    float health;
    float attack;
    float defense;
    float evasionRate;
    bool defending;

    int gold;
} Stats;

typedef struct Inventario{
    Lista* utils;
    Lista* weapons;
} Inventario;

typedef struct Player{
    Texture2D spriteSheet;

    Animation* anim;
    
    Vector2 display;
    Vector2 coordinates;

    Rectangle source;
    Rectangle destination;

    float stepDistance;
    bool characterChoice;

    MoveSet keys;
    Moving moving;

    Stats stats;
    Inventario inventario;
} Player;

Player* Player_Init(Rectangle source, Rectangle destination, const char* spriteSheet);

void Player_UpdateAtk(Player* player, float newAtk);

void Player_UpdateDef(Player* player, float newDef);

Vector2 Player_GetCoords(Player* player);

void Player_SetStepSize(Player* player, float stepSize);

void Player_Update(Player* player, float deltaTime);

void Player_SetMoveSet(Player* player, MoveSet moveSet);

void Player_UpdatePosition(Player* player, float deltaTime);

void Player_UpdateSize(Player* player, float deltaTime);

void Player_ChangeCharacter(Player* player);

void Player_UpdateSprite(Player* player);

void Player_StepTo(Player* player, Vector2 direction, bool updateSprite);

void Player_MoveTo(Player* player, Vector2 newPos, float interval);

void Player_Resize(Player* player, float ratio, float interval);

void Player_SetSpriteSheet(Player* player, const char* spriteSheet);

void Player_SetSourceRec(Player* player, Rectangle newSource);

void Player_SetDestRec(Player* player, Rectangle newDest);

void Player_SetSpriteSize(Player* player, int width, int height);

void Player_Draw(Player* player);

///////////////////////////////////////////////////////////////////////

bool isLife50(Player* player);

bool isLife30(Player* player);

//////////////////////////////////////////////////////////////////////////

void Player_TakeDamage(Player* player, float damage);

void Player_UpdateAtk(Player* player, float newAtk);

void Player_UpdateDef(Player* player, float newDef);

void Player_getHealing(Player* player);

void Player_Print(Player* player);

#endif