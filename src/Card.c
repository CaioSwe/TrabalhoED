#include "Card.h"

Card* Card_Init(Rectangle rect, Texture2D cardImage){
    Card* card = (Card*)malloc(sizeof(Card));
    card->anim = (Anim*)malloc(sizeof(Anim));
    card->anim->pos = (PositionAnimation*)malloc(sizeof(PositionAnimation));
    card->anim->scale = (ScaleAnimation*)malloc(sizeof(ScaleAnimation));

    card->rect = rect;
    card->rectOriginal = rect;
    card->img = cardImage;
    card->attached = false;
    card->hovered = false;

    card->anim->scale->maximized = true;

    card->cor = (Color){(rand() % 255), (rand() % 255), (rand() % 255), 255};

    return card;
}

void moveCard(Card* card, bool maximizeOnGrab){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mousepos = GetMousePosition();

        if(CheckCollisionPointRec(mousepos, card->rect)){
            card->attached = true;
        }
    }

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        card->attached = false;
    }

    if(card->attached){
        if(maximizeOnGrab){
            CardMaximize(card);
        }

        card->rect.x += GetMouseDelta().x;
        card->rect.y += GetMouseDelta().y;
    }
}

void MoveCardTo(Card* card, Vector2 finalPoint, float time){
    card->anim->pos->start = (Vector2){card->rect.x + card->rect.width/2, card->rect.y + card->rect.height/2};
    card->anim->pos->end = finalPoint;
    card->anim->pos->current = (Vector2){card->rect.x, card->rect.y};
    card->anim->pos->duration = time;
    card->anim->pos->elapsed = 0;
    card->anim->pos->animating = true;
}

void UpdateCardPosAnim(Card* card, float deltaTime, bool centerAligned){
    if(card->anim->pos->animating){
        card->anim->pos->elapsed += deltaTime;

        float progress = card->anim->pos->elapsed / card->anim->pos->duration;
        if (progress >= 1.0f) {
            progress = 1.0f;
            card->anim->pos->animating = false;
        }
        float easedProgress = 1 - powf(1 - progress, 3);

        if(centerAligned){
            Vector2 centerStart = (Vector2){card->anim->pos->start.x, card->anim->pos->start.y};
            
            card->anim->pos->current.x = Slerp(centerStart.x, card->anim->pos->end.x, easedProgress);
            card->anim->pos->current.y = Slerp(centerStart.y, card->anim->pos->end.y, easedProgress);
            card->rect.x = card->anim->pos->current.x - card->rect.width/2;
            card->rect.y = card->anim->pos->current.y - card->rect.height/2;
            card->rectOriginal.x = card->rect.x;
            card->rectOriginal.y = card->rect.y;
        }
        else{
            card->anim->pos->current.x = Slerp(card->anim->pos->start.x, card->anim->pos->end.x, easedProgress);
            card->anim->pos->current.y = Slerp(card->anim->pos->start.y, card->anim->pos->end.y, easedProgress);
            card->rect.x = card->anim->pos->current.x;
            card->rect.y = card->anim->pos->current.y;
            card->rectOriginal.x = card->rect.x;
            card->rectOriginal.y = card->rect.y;
        }
    }
}

void CardMinimize(Card* card){
    ScaleAnimation* temp = card->anim->scale;

    if(temp->animating && temp->maximized){
        temp->maximized = false;
        temp->minimized = true;
        temp->animating = false;
        
        float centerX = card->rect.x + card->rect.width/2;
        float centerY = card->rect.y + card->rect.height/2;

        card->rect.width = card->rectOriginal.width / temp->end;
        card->rect.height = card->rectOriginal.height / temp->end;
        card->rect.x = centerX - card->rect.width/2;
        card->rect.y = centerY - card->rect.height/2;

        printf("\n | Triggered");
    }
    if(!temp->minimized){
        temp->start = 1.0f;
        temp->end = 1.0f * 0.5f;
        temp->current = 1.0f;
        temp->duration = 0.2f;
        temp->elapsed = 0;
        temp->animating = true;

        temp->minimized = true;
        temp->maximized = false;
    }
}

void CardMaximize(Card* card){
    ScaleAnimation* temp = card->anim->scale;

    if(temp->animating && temp->minimized){
        temp->maximized = true;
        temp->minimized = false;
        temp->animating = false;

        float centerX = card->rect.x + card->rect.width/2;
        float centerY = card->rect.y + card->rect.height/2;

        card->rect.width = card->rectOriginal.width * temp->end;
        card->rect.height = card->rectOriginal.height * temp->end;
        card->rect.x = centerX - card->rect.width/2;
        card->rect.y = centerY - card->rect.height/2;

        printf("\n | Triggered");
    }
    if(!temp->maximized){
        temp->start = 1.0f;
        temp->end = 1.0f / 0.5f;
        temp->current = 1.0f;
        temp->duration = 0.2f;
        temp->elapsed = 0;
        temp->animating = true;

        temp->maximized = true;
        temp->minimized = false;
    }
}

void isCardHover(Card* card, Vector2 mousepos){
    if(CheckCollisionPointRec(mousepos, card->rect)){
        card->hovered = true;
    }
    else{
        card->hovered = false;
    }
}

void CardWiggle(Card* card){
    card->rotation = sin(GetTime() * 18) * PI/2;
}

void UpdateCardSize(Card* card, float deltaTime){
    if(card->anim->scale->animating){
        printf("\nX = %.2f", card->rect.x);
        printf(", Y = %.2f", card->rect.y);

        card->anim->scale->elapsed += deltaTime;

        float progress = card->anim->scale->elapsed / card->anim->scale->duration;

        if (progress >= 1.0f) {
            progress = 1.0f;
            card->anim->scale->animating = false;
        }

        float easedProgress = 1 - powf(1 - progress, 3);

        float prevScale = card->anim->scale->current;
        float newScale = Slerp(card->anim->scale->start, card->anim->scale->end, easedProgress);

        float centerX = card->rect.x + card->rect.width/2;
        float centerY = card->rect.y + card->rect.height/2;

        card->anim->scale->current = newScale;

        float baseWidth = card->rect.width / prevScale;
        float baseHeight = card->rect.height / prevScale;
        card->rect.width = baseWidth * newScale;
        card->rect.height = baseHeight * newScale;

        card->rect.x = centerX - card->rect.width/2;
        card->rect.y = centerY - card->rect.height/2;
    }
}

void Card_Draw(Card* card){
    card->img.width = card->rect.width;
    card->img.height = card->rect.height;

    Rectangle source = (Rectangle){0, 0, card->rect.width, card->rect.height};
    Rectangle dest = (Rectangle){
        card->rect.x + card->img.width / 2.0f,
        card->rect.y + card->img.height / 2.0f,
        card->img.width,
        card->img.height
    };
    Vector2 origin = (Vector2){card->img.width / 2.0f, card->img.height / 2.0f};

    DrawTexturePro(card->img, source, dest, origin, card->rotation, WHITE);
}
