#include "Animation.h"

void MoveItemTo(PositionAnimation* item, Vector2 startPoint, Vector2 finalPoint, float duration){
    item->start = startPoint;
    item->end = finalPoint;
    item->current = item->start;
    item->duration = duration;
    item->elapsed = 0;
    item->animating = true;
}

Vector2 UpdateItemPosition(PositionAnimation* item, float deltaTime){
    Vector2 current = item->end;

    if(item->animating){
        item->elapsed += deltaTime;

        float progress = item->elapsed / item->duration;
        if(progress >= 1.0f){
            progress = 1.0f;
            item->animating = false;
        }
        
        float eased = item->function(progress);

        current.x = Slerp(item->start.x, item->end.x, eased);
        current.y = Slerp(item->start.y, item->end.y, eased);

        return current;
    }

    return current;
}

void ResizeItem(ScaleAnimation* item, float newSize, float interval){
    if(!item->animating){
        item->start = 1.0f;
        item->end = 1.0f * newSize;
        item->current = 1.0f;
        item->duration = interval;
        item->elapsed = 0;
        item->animating = true;
    }
}

Rectangle UpdateItemSize(ScaleAnimation* item, Rectangle recItem, float deltaTime) {
    if (item->animating) {
        item->elapsed += deltaTime;

        float progress = item->elapsed / item->duration;
        if (progress >= 1.0f) {
            progress = 1.0f;
            item->animating = false;
        }

        float easedProgress = 1 - powf(1 - progress, 3);
        float prevScale = item->current;
        float newScale = Slerp(item->start, item->end, easedProgress);
        item->current = newScale;

        float centerX = recItem.x + recItem.width / 2.0f;
        float centerY = recItem.y + recItem.height / 2.0f;

        float baseWidth = recItem.width / prevScale;
        float baseHeight = recItem.height / prevScale;

        recItem.width = baseWidth * newScale;
        recItem.height = baseHeight * newScale;

        recItem.x = centerX - recItem.width / 2.0f;
        recItem.y = centerY - recItem.height / 2.0f;
    }

    return recItem;
}

void ChangePositionFunction(PositionAnimation* item, float (*function)(float)){
    item->function = function;
}