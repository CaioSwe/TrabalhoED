#ifndef _Card_h_
#define _Card_h_

#include "Utils.h"
#include "Delimiters.h"

#include "ImageObject.h"
#include "Animation.h"

typedef struct Anim {
    PositionAnimation* pos;
    ScaleAnimation* scale;
} Anim;

typedef struct Card {
    Texture2D img;
    Anim* anim;

    Rectangle rect;
    Rectangle rectOriginal;
    
    bool hovered;
    bool attached;

    float rotation;

    Color cor;
} Card;

Card* Card_Init(Rectangle rect, Texture2D path);

void moveCard(Card* card, bool minimizeOnGrab);

void MoveCardTo(Card* card, Vector2 finalPoint, float time);

void UpdateCardPosAnim(Card* card, float deltaTime, bool centerAligned);

void CardWiggle(Card* card);

void CardMinimize(Card* card);

void CardMaximize(Card* card);

void UpdateCardSize(Card* card, float deltaTime);

void Card_Draw(Card* card);

#endif