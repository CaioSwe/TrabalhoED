#ifndef _Animation_h_
#define _Animation_h_

#include "Utils.h"
#include "Delimiters.h"

typedef struct PositionAnimation {
    float elapsed;
    bool animating;
    float duration;

    float (*function)(float);

    Vector2 current;
    Vector2 start;
    Vector2 end;
} PositionAnimation;

typedef struct ScaleAnimation {
    float elapsed;
    bool animating;
    float duration;

    bool maximized;
    bool minimized;

    float current;
    float start;
    float end;
} ScaleAnimation;

typedef struct FramesAnimation{
    bool animating;

    float framesPadding;
    float padding;

    int animationDirection;
    int currentFrame;
    int amountOfFrames;
    int framesCounter;

    int framesSpeed;
    int baseSpeed;
} FramesAnimation;

typedef struct Turn{
    bool animationEnd;
    bool animationBegin;
    bool animationBool;

    bool animating;
} Turn;

void MoveItemTo(PositionAnimation* item, Vector2 startPoint, Vector2 finalPoint, float duration);

Vector2 UpdateItemPosition(PositionAnimation* item, float deltaTime);

void ResizeItem(ScaleAnimation* item, float newSize, float interval);

Rectangle UpdateItemSize(ScaleAnimation* item, Rectangle recItem, float deltaTime);

void ChangePositionFunction(PositionAnimation* item, float (*function)(float));

#endif