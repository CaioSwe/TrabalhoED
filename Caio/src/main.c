#pragma region "Structs"

#include "Button.h"
#include "ImageObject.h"
#include "Gif.h"
#include "TextObject.h"
#include "Input.h"
#include "Player.h"
#include "TileSet.h"
#include "Game.h"
#include "SpriteSheet.h"

#include "movimentacao.h"
#include "ItensColeta.h"

#include "Pilha.h"
#include "Lista.h"
#include "Arvore.h"

#include "Mapa.h"

typedef struct CutsceneFunctions{
    float (*function)(float);
    void (*target)(float);
    void (*reset)();
} CutsceneFunctions;

typedef enum GAMESTATE{
    MAINSCREEN,
    INTRODUCTION,
    FREE,
    BATTLE,
    ENDSCREEN
} GAMESTATE;

typedef struct Resources{
    Player* player;
    Player* enemy;
    float squareSize;
    Vector2* playerCoordsSave;
    int** mapa;
    ImageObject* cursor;
    ImageObject* grainOverlay;
} Resources;

typedef struct healedOrHurt{
    bool healed;
    bool hurt;
} healedOrHurt;

#pragma endregion "Structs"

Camera2D camera = {0};
float cameraInitZoom = TAM / 8;

Lista* itens;
Lista* inimigos;
Lista* traps;

bool deletedMisteryBox = false;
bool collidedWithEnemy = false;
bool collidedWithTrap = false;

void moveRandom(Player* instance, double interval){
    if(Wait(interval)){
        Player_StepTo(instance, (Vector2){(rand() % 3) - 1, (rand() % 2) - 1}, true);
    }
}

#pragma region "VoidPointers"

void initAllLists(){
    Button_InitList();
}

void freeAllLists(){
    Button_Free();
}

void imprimirPlayer(const void* p){
    const Player* player = (const Player*)p;
    Player_Draw((Player*)player);
}

void imprimirImageObject(const void* item) {
    const ImageObject* image = (const ImageObject*)item;
    Image_Draw((ImageObject*)image);
}

void imprimirImageObjectPro(const void* item) {
    const ImageObject* image = (const ImageObject*)item;
    Image_DrawPro((ImageObject*)image);
}

bool compararItem(const void* item, const void* comparar){
    const Item* card = (const Item*)item;
    const Item* comp = (const Item*)comparar;

    if(card == comp){
        return true;
    }
    return false;
}

bool compararPlayer(const void* item, const void* comparar){
    const Player* p = (const Player*)item;
    const Player* comp = (const Player*)comparar;

    if(p == comp){
        return true;
    }
    return false;
}

void imprimirImageObjectProBH(const void* item, const void* target) {
    const ImageObject* image = item;
    const Player* player = target;

    if(image->save == 0){
        Image_DrawPro((ImageObject*)image);
        return;
    }

    Rectangle playerDestRec = Player_getDestRec((Player*)player);
    Rectangle imgRec = image->destination;

    bool intersects = CheckCollisionRecs(imgRec, playerDestRec);
    bool isBehind = (imgRec.y + imgRec.height) <= (playerDestRec.y + playerDestRec.height);

    if (intersects && isBehind) {
        Image_DrawPro((ImageObject*)image);
    } else if (!intersects) {
        imprimirImageObjectPro(image);
    }
}

void imprimirImageObjectProFW(const void* item, const void* target) {
    const ImageObject* image = item;
    const Player* player = target;

    if(image->save == 0) return;

    Rectangle playerDestRec = Player_getDestRec((Player*)player);
    Rectangle imgRec = image->destination;

    bool intersects = CheckCollisionRecs(imgRec, playerDestRec);
    bool isFront = (image->save == 1) ? (imgRec.y + imgRec.height) >= (playerDestRec.y + playerDestRec.height) : (imgRec.y + imgRec.height) > (playerDestRec.y + playerDestRec.height);

    if (intersects && isFront) {
        Image_DrawPro((ImageObject*)image);
    }
}


void imprimirItensInventario(const void* item, const void* target, const void* p){
    static int t = 0;
    
    const Item* obj = (const Item*)item;
    const ImageObject* inventoryBackground = (const ImageObject*)target;
    const Player* player = (const Player*)p;

    float ratioWidth = (float)inventoryBackground->destination.width/inventoryBackground->source.width;
    float ratioHeight = (float)inventoryBackground->destination.height/inventoryBackground->source.height;

    float slotDeltaX = 72.0f * ratioWidth;

    Rectangle slotRec = {31.0f * ratioWidth, 363.0f * ratioHeight, 64.0f * ratioWidth, 64.0f * ratioHeight};

    ((Item*)obj)->sprite->destination = (Rectangle){slotRec.x + inventoryBackground->destination.x + slotDeltaX*t, slotRec.y + inventoryBackground->destination.y, slotRec.width, slotRec.height};
    Image_DrawPro(((Item*)obj)->sprite);
    
    t += 1;

    if(t == listaTamanho(Player_getInventarioUtils((Player*)player))) t = 0;
}

void imprimirItensDescInventario(const void* item, const void* target){
    const Item* obj = (const Item*)item;
    const Player* player = (const Player*)target;

    Vector2 mousePos = GetMousePosition();

    if(CheckCollisionPointRec(mousePos, (((Item*)obj)->sprite->destination))){
        DrawRectangle(mousePos.x, mousePos.y - 50.0f, MeasureText(getItemDescription((Item*)obj), 20), 50.0f, WHITE); // TEXT BOX
        DrawText(getItemName((Item*)obj), mousePos.x, mousePos.y - 45.0f, 20, BLACK);
        DrawText(TextFormat("\n%s", getItemDescription((Item*)obj)), mousePos.x, mousePos.y - 45.0f, 20, BLACK);

        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) printf("\n%s", getItemDescription((Item*)obj)); // ITEM USAGE
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) usarItem2(((Item*)obj), (Player*)player);
    }
}

void deleteUsedItems(const void* item, const void* target){
    const Item* obj = (const Item*)item;
    const Player* player = (const Player*)target;

    if(((Item*)obj)->used) remover(Player_getInventarioUtils((Player*)player), compararItem, ((Item*)obj), true);
}

void isCollidingWithPlayer(const void* item, const void* target){
    const ImageObject* image = (const ImageObject*)item;
    const Player* player = (const Player*)target;

    Rectangle playerDestRec = Player_getDestRec((Player*)player);

    if(CheckCollisionRecs(playerDestRec, image->destination)){       
        remover(itens, compararItem, item, true);
        deletedMisteryBox = true;
    }
}

void isCollidingWithPlayerEnemy(const void* item, const void* target){
    const ImageObject* image = (const ImageObject*)item;
    const Player* player = (const Player*)target;

    Rectangle playerDestRec = Player_getDestRec((Player*)player);

    if(CheckCollisionRecs(playerDestRec, image->destination) && Player_getStats((Player*)player).repelent <= 0){       
        remover(inimigos, compararItem, item, true);
        collidedWithEnemy = true;
    }
}

void isCollidingWithPlayerTrap(const void* item, const void* target){
    const ImageObject* image = (const ImageObject*)item;
    const Player* player = (const Player*)target;

    Rectangle playerDestRec = Player_getDestRec((Player*)player);

    if(CheckCollisionRecs(playerDestRec, image->destination)){       
        remover(traps, compararItem, item, true);
        collidedWithTrap = true;
    }
}

void formatButtons(const void* item){
    Button* button = (Button*)item;

    Button_FitSizeToText(button, 15, (Vector2){3, 3});
    button->fontsize = 12;
    Button_Deactivate(button);
}

void Button_DrawIconVoid(const void* item){
    Button* btn = (Button*)item;

    Rectangle rect = {btn->x - btn->padding.x/2, btn->y - btn->padding.y, btn->width + btn->padding.x, btn->height + btn->padding.y};
    Vector2 mousePos = GetMousePosition();

    DrawRectangleRec(rect, btn->colors.backgroundColor);

    if (!Button_Hovering(btn, mousePos) || !btn->active){
        btn->activeColor = (ActivePattern){btn->colors.baseColor, btn->colors.borderColor, btn->colors.backgroundColor};
        DrawRectangleLinesEx(rect, btn->borda, btn->colors.borderColor);
        DrawText(btn->text, btn->x + btn->width * 3/5 - MeasureText(btn->text, btn->fontsize) / 2, btn->y + btn->height / 2 - btn->fontsize / 2, btn->fontsize, btn->colors.baseColor);
        btn->icon->color = btn->colors.baseColor;
    } else{
        btn->activeColor = (ActivePattern){btn->colors.hoverColor, btn->colors.borderHoverColor, btn->colors.backgroundColor};
        DrawRectangleRec(rect, btn->colors.borderHoverColor);
        DrawText(btn->text, btn->x + btn->width * 3/5 - MeasureText(btn->text, btn->fontsize) / 2, btn->y + btn->height / 2 - btn->fontsize / 2, btn->fontsize, btn->colors.hoverColor);
        btn->icon->color = btn->colors.hoverColor;
    }
    Image_DrawPro(btn->icon);
}

bool isLife50Void(const void* item){
    return isLife50((Player*)item);
}

bool isLife30Void(const void* item){
    return isLife30((Player*)item);
}

bool Player_PrintVoid(const void* item){
    Player_Print((Player*)item);
    return true;
}

bool attackVoid(const void* item){
    Player_setAction((Player*)item, ATTACK);
    return true;
}

void upDownFunction(const void* item, const void* delta){
    ImageObject* image = (ImageObject*)item;

    if(image == NULL) return;

    const float* deltaTime = (const float*)delta;
    float* elapsed = &image->elapsed;

    *elapsed += *deltaTime;
    
    float pg = *elapsed / 2.0f;
    float t = cos(2*3.14*pg);

    image->destination.y = Slerp(image->save, image->save + 3, t);

    if(pg > 1.0f){
        elapsed = 0;
    }
}

#pragma endregion "VoidPointers"

#pragma region "Funcoes Uteis"

void drawLightSource(Vector2 center, float radius, float strength){
    int value = 255 * (1.0f - (strength / 100.0f));

    BeginBlendMode(BLEND_MULTIPLIED);
        DrawCircleGradient(center.x, center.y, radius, (Color){255, 255, 255, value}, (Color){255, 255, 255, 255});
        DrawCircleGradient(center.x, center.y, radius, (Color){255, 255, 255, value}, (Color){255, 255, 255, 255});
        DrawCircleGradient(center.x, center.y, radius, (Color){255, 255, 255, value}, (Color){255, 255, 255, 255});
    EndBlendMode();
}

void battleTransitionF(float t){
    static Color cover = {0, 0, 0, 0};

    cover.a = Slerp(0, 255, t);
    camera.zoom = cameraInitZoom * (t + 1);

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), cover);
}

void battleTransitionR(){
    camera.zoom = cameraInitZoom;
}

bool cutsceneListener(bool activate, float duration, float deltaTime, CutsceneFunctions functions){
    if(!activate) return false;

    static float elapsed = 0;
    static float timePassed = 0;

    timePassed += deltaTime;

    if(timePassed/1.0f < 1.0f) return false;

    elapsed += deltaTime;
    
    float pg = elapsed / duration;
    float t = functions.function(pg);

    functions.target(t);

    if(pg > 1.0f){
        activate = false;
        elapsed = 0;
        timePassed = 0;
        functions.reset();

        return true;
    }

    return false;
}

void generateGrass(Lista* lista, ImageObject* spriteSheet, Vector2 coords, float squareSize){
    for(int i = 0; i < 2; i++){
        ImageObject* grama = Image_Init(NULL);

        Image_Copy(spriteSheet, grama, true);

        grama->source.x = i * spriteSheet->source.width;
        grama->destination.x = squareSize*coords.y;
        grama->destination.y = squareSize*coords.x + (squareSize - grama->destination.height);

        grama->save = i;

        inserirFim(lista, grama);
    }
}

Turn openInventory(ImageObject* background, float deltaTime){
    static bool inventoryOpened = false;
    static float elapsed = 0;
    static bool animBegan = false;

    animBegan = (elapsed == 0);

    elapsed += deltaTime;
    
    float pg = (inventoryOpened) ? elapsed / 0.6f : elapsed / 0.8f;
    float t = (inventoryOpened) ? -pow(pg, 8.0f) + 1 : -pow(pg-1, 8.0f) + 1;

    background->destination.y = Slerp(GetScreenHeight() + background->destination.height*0.2f, GetScreenHeight()/2 - background->destination.height/2, t);

    if(pg > 1.0f){
        elapsed = 0;
        inventoryOpened = !inventoryOpened;

        return (Turn){true, false, inventoryOpened, false};
    }
    return (Turn){false, animBegan, inventoryOpened, true};
}

Turn openChest(ImageObject* background, float deltaTime){
    static bool inventoryOpened = false;
    static float elapsed = 0;
    static bool animBegan = false;

    animBegan = (elapsed == 0);

    elapsed += deltaTime;
    
    float pg = (inventoryOpened) ? elapsed / 0.6f : elapsed / 0.8f;
    float t = (inventoryOpened) ? -pow(pg, 8.0f) + 1 : -pow(pg-1, 8.0f) + 1;

    background->destination.y = Slerp(GetScreenHeight() + background->destination.height*0.2f, GetScreenHeight()*3/7 - background->destination.height/2, t);

    if(pg > 1.0f){
        elapsed = 0;
        inventoryOpened = !inventoryOpened;

        return (Turn){true, false, inventoryOpened, false};
    }
    return (Turn){false, animBegan, inventoryOpened, true};
}

Turn changeTurn(float deltaTime){
    static bool playersTurn = false;
    static float elapsed = 0;

    static Rectangle yourTurn = (Rectangle){0, SCREEN_HEIGHT/2, SCREEN_WIDTH, 0};
    
    TextObject* yourTurnText = (playersTurn) ? Text_Init("Your Turn!") : Text_Init("Enemys Turn!");
    yourTurnText->fontsize = yourTurn.height*2/3;
    
    Text_Pos(yourTurnText, (Vector2){GetScreenWidth()/2 - MeasureText(yourTurnText->text, yourTurnText->fontsize)/2, yourTurn.y + yourTurn.height/2 - yourTurnText->fontsize/2});

    bool animBegan = (elapsed == 0);

    elapsed += deltaTime;
    
    float pg = elapsed / 2.0f;
    float t = -pow((2*pg)-1, 8.0f) + 1;

    yourTurn.height = Slerp(0, GetScreenHeight()/8, t);
    yourTurn.y = Slerp(GetScreenHeight()/2, GetScreenHeight()/2 - GetScreenHeight()/16, t);

    if(pg > 1.0f){
        elapsed = 0;
        yourTurn = (Rectangle){0, SCREEN_HEIGHT/2, SCREEN_WIDTH, 0};
        playersTurn = !playersTurn;
        return (Turn){true, false, !playersTurn, false};
    }
    DrawRectangleRec(yourTurn, (Color){0, 0, 0, 180});
    Text_DrawS(yourTurnText);
    free(yourTurnText);
    return (Turn){false, animBegan, playersTurn, true};
}

bool flashScreenHurt(float deltaTime){
    static float elapsed = 0;
    static Rectangle filler = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    static Color fillerColor = {255, 0, 0, 0};

    elapsed += deltaTime;

    float pg = elapsed / 0.5f;
    float t = (-pg+1)/2.0f;

    fillerColor.a = Slerp(0, 255, t);

    if(pg > 1.0f){
        elapsed = 0;
        fillerColor.a = 0;
        return true;
    }

    DrawRectangleRec(filler, fillerColor);
    return false;
}

bool hurtAnimation(Rectangle* rec, Vector2 initialPos, Color* healthBarBorderColor, float deltaTime){
    static float elapsed = 0;
    
    elapsed += deltaTime;
    
    float pg = elapsed / 2.0f;
    float t = (cos(10.0f/(pg+0.25))/(20.0f*pg + 0.8f));
    float t2 = -pow((2*pg)-1, 6.0f) + 1;

    rec->x = Slerp(initialPos.x, initialPos.x + 10, t);
    healthBarBorderColor->r = Slerp(255, 0, t2);

    if(pg > 1.0f){
        elapsed = 0;
        healthBarBorderColor->r = 255;
        return true;
    }

    return false;
}

bool healedAnimation(Color* healthBarColor, float deltaTime){
    static float elapsed = 0;

    elapsed += deltaTime;
    
    float pg = elapsed / 2.0f;
    float t = pow(sin(3*3.14*pg), 2);
    
    healthBarColor->r = Slerp(255, 0, t);
    healthBarColor->b = Slerp(255, 0, t);

    if(pg > 1.0f){
        elapsed = 0;
        healthBarColor->b = 255;
        healthBarColor->r = 255;
        return true;
    }

    return false;
}

healedOrHurt healthBarP(ImageObject* healthBarSprite, ImageObject* healthBarFiller, Vector2 initialPos, float health, float maxHealth, float deltaTime){
    static float relativeHealth = 10; // MUDAR
    static float elapsed = 0;
    static bool hurt = false;
    static bool hurt2 = false;
    static bool healed = false;
    static Color healthBarColor = WHITE;
    static Color healthBarBorderColor = RED;

    bool stateHurt = (health < relativeHealth);
    bool stateHeal = (health > relativeHealth);

    if(health < relativeHealth){
        relativeHealth = health;
        hurt = true;
        hurt2 = true;
    }
    if(health > relativeHealth){
        relativeHealth = health;
        healed = true;
    }

    if(hurt2){
        if(flashScreenHurt(deltaTime)){
            hurt2 = false;
        }
    }

    if(hurt){
        if(hurtAnimation(&healthBarSprite->destination, (Vector2){healthBarSprite->destination.x, healthBarSprite->destination.y}, &healthBarBorderColor, deltaTime)){
            hurt = false;
        }
    }
    if(healed){
        if(healedAnimation(&healthBarColor, deltaTime)){
            healed = false;
        }
    }

    elapsed += deltaTime;
    
    float pg = elapsed / 2.0f;
    float t = cos(2*3.14*pg);

    healthBarSprite->destination.y = Slerp(initialPos.y, initialPos.y + 3, t);

    if(pg > 1.0f){
        elapsed = 0;
    }

    healthBarSprite->color = healthBarColor;
    healthBarFiller->color = healthBarBorderColor;

    healthBarFiller->destination = healthBarSprite->destination;

    healthBarFiller->destination.x = healthBarSprite->destination.x + 1.0f;
    healthBarFiller->source.width = (healthBarSprite->image.width * health) / maxHealth;
    healthBarFiller->destination.width = (healthBarSprite->destination.width * health) / maxHealth;

    return (healedOrHurt){stateHeal, stateHurt};
}

healedOrHurt healthBar(ImageObject* healthBarSprite, ImageObject* healthBarFiller, Vector2 initialPos, float health, float maxHealth, float deltaTime){
    static float relativeHealth = 10; // MUDAR
    static float elapsed = 0;
    static bool hurt = false;
    static bool healed = false;
    static Color healthBarColor = WHITE;
    static Color healthBarBorderColor = RED;

    bool stateHurt = (health < relativeHealth);
    bool stateHeal = (health > relativeHealth);

    if(health < relativeHealth){
        relativeHealth = health;
        hurt = true;
    }
    if(health > relativeHealth){
        relativeHealth = health;
        healed = true;
    }

    if(hurt){
        if(hurtAnimation(&healthBarSprite->destination, (Vector2){healthBarSprite->destination.x, healthBarSprite->destination.y}, &healthBarBorderColor, deltaTime)){
            hurt = false;
        }
    }
    if(healed){
        if(healedAnimation(&healthBarColor, deltaTime)){
            healed = false;
        }
    }

    elapsed += deltaTime;
    
    float pg = elapsed / 2.0f;
    float t = cos(2*3.14*pg);

    healthBarSprite->destination.y = Slerp(initialPos.y, initialPos.y + 3, t);

    if(pg > 1.0f){
        elapsed = 0;
    }

    healthBarSprite->color = healthBarColor;
    healthBarFiller->color = healthBarBorderColor;

    healthBarFiller->destination = healthBarSprite->destination;

    healthBarFiller->destination.x = healthBarSprite->destination.x + 1.0f;
    healthBarFiller->source.width = (healthBarSprite->image.width * health) / maxHealth;
    healthBarFiller->destination.width = (healthBarSprite->destination.width * health) / maxHealth;

    return (healedOrHurt){stateHeal, stateHurt};
}

void LogHealthP(float health, TextObject* text, Rectangle position, float deltaTime){
    static float relativeHealth = 10; // MUDAR
    static float elapsed = 0;
    static bool hurt = false;
    static bool healed = false;
    static Color damageColor = RED;
    static float showHealth = 0;

    if(health < relativeHealth){
        showHealth = relativeHealth - health;
        relativeHealth = health;
        hurt = true;
    }
    if(health > relativeHealth){
        showHealth = relativeHealth - health;
        relativeHealth = health;
        healed = true;
    }

    if(!hurt && !healed) return;

    if(hurt) Text_Set(text, TextFormat("-%.1f", showHealth));
    if(healed) Text_Set(text, TextFormat("+%.1f", showHealth));

    damageColor = (hurt) ? RED : (healed) ? GREEN : GRAY;

    elapsed += deltaTime;
    
    float pg = elapsed / 2.0f;
    float t = pg;

    text->y = Slerp(position.y, position.y - 100, t);
    int alpha = Slerp(255, 0, t);
    text->color = (Color){damageColor.r, damageColor.g, damageColor.b, alpha};

    if(pg > 1.0f){
        elapsed = 0;
        hurt = false;
        healed = false;
    }

    text->x = position.x;
    text->fontsize = position.height;

    Text_DrawS(text);
}

void LogHealthE(float health, TextObject* text, Rectangle position, float deltaTime){
    static float relativeHealth = 10; // MUDAR
    static float elapsed = 0;
    static bool hurt = false;
    static bool healed = false;
    static Color damageColor = RED;
    static float showHealth = 0;

    if(health < relativeHealth){
        showHealth = relativeHealth - health;
        relativeHealth = health;
        hurt = true;
    }
    if(health > relativeHealth){
        showHealth = relativeHealth - health;
        relativeHealth = health;
        healed = true;
    }

    if(!hurt && !healed) return;

    if(hurt) Text_Set(text, TextFormat("-%.1f", showHealth));
    if(healed) Text_Set(text, TextFormat("+%.1f", showHealth));

    damageColor = (hurt) ? RED : (healed) ? GREEN : GRAY;

    elapsed += deltaTime;
    
    float pg = elapsed / 2.0f;
    float t = pg;

    text->y = Slerp(position.y, position.y - 100, t);
    int alpha = Slerp(255, 0, t);
    text->color = (Color){damageColor.r, damageColor.g, damageColor.b, alpha};

    if(pg > 1.0f){
        elapsed = 0;
        hurt = false;
        healed = false;
    }

    text->x = position.x;
    text->fontsize = position.height;

    Text_DrawS(text);
}

bool cameraAnimation(Camera2D* camera, Vector2 startPoint, Vector2 zoomPoint, float deltaTime){
    static bool zoomedIn = true;
    static float elapsed = 0;

    elapsed += deltaTime;
    
    float pg = (zoomedIn) ? elapsed / 2.0f : elapsed / 1.0f;
    float t = (zoomedIn) ? -pow(((2*pg)-2)/2.0f, 6.0f) + 1 : -pow(((2*pg))/2.0f, 6.0f) + 1;
    
    camera->target.x = Slerp(startPoint.x, zoomPoint.x, t);
    camera->target.y = Slerp(startPoint.y, zoomPoint.y, t);

    camera->zoom = Slerp(1.0f, 1.8f, t);

    if(pg > 1.0f){
        elapsed = 0;
        zoomedIn = !zoomedIn;
        return true;
    }
    return false;
}

#pragma endregion "Funcoes Uteis"

int menuOpen(){
    Button* restart = Button_Init("Reiniciar");
    Button* homeButton = Button_Init("Voltar à tela inicial");
    Button* exitButton = Button_Init("Sair");

    float xButton = 0.5f;
    float yButton = 0.5f;

    Button_FitSizeToText(homeButton, 30, (Vector2){4, 20});

    homeButton->x = (GetScreenWidth()*xButton - (homeButton->width)/2);
    homeButton->y = (GetScreenHeight()*yButton) - (homeButton->height)/2;

    Button_SetPattern(homeButton, (Pattern){WHITE, WHITE, WHITE, WHITE, BLACK});

    Button_FitSizeToText(exitButton, 30, (Vector2){20, 20});

    exitButton->x = (GetScreenWidth()*xButton - (exitButton->width)/2);
    exitButton->y = homeButton->y + (homeButton->height + homeButton->padding.y) *1.3f;

    Button_SetPattern(homeButton, (Pattern){(Color) {112, 21, 41, 255}, (Color) {112, 21, 41, 255}, WHITE, WHITE, BLACK});

    Button_FitSizeToText(restart, 30, (Vector2){4, 20});

    restart->x = (GetScreenWidth()*xButton - (restart->width)/2);
    restart->y = homeButton->y - (homeButton->height + homeButton->padding.y) *1.3f;

    Button_SetPattern(restart, (Pattern){WHITE, WHITE, WHITE, WHITE, BLACK});

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangle(0,0,GetScreenWidth(), GetScreenHeight(), (Color) {0,0,0,150});

            Button_Draw(restart);
            Button_Draw(homeButton);
            Button_Draw(exitButton);
        EndDrawing();
        
        Vector2 mouse = GetMousePosition();

        if(Button_IsPressed(restart, mouse)){
            free(restart);
            free(homeButton);
            free(exitButton);

            return 1;
        }
        if(Button_IsPressed(homeButton, mouse)){
            free(restart);
            free(homeButton);
            free(exitButton);

            return 2;
        }
        if(Button_IsPressed(exitButton, mouse)){
            free(restart);
            free(homeButton);
            free(exitButton);

            return 3;
        }
        if(IsKeyPressed(KEY_ESCAPE)){
            free(restart);
            free(homeButton);
            free(exitButton);

            return 0;
        }
    }
    return 0;
}

GAMESTATE fightScreen(Resources resources){
    Player* player = Player_CopyF(resources.player);
    Player* enemy = Player_CopyF(resources.enemy);

    // bc (1) cima
    // bb (1) baixo
    // bp (2) baixo
    // nv (3) cima
    // nn (3) baixo

    ImageObject* bc = Image_Init("sprites/parallax.png");
    bc->source = (Rectangle){0, 0, 2560.0f, 640.0f};

    float bc_height = ((float)GetScreenWidth()*bc->source.height)/bc->source.width;

    float startPointC = 0-bc_height/2;
    float startPointB = GetScreenHeight()-bc_height;
    
    float finalPointC = 0-bc_height/2;
    float finalPointB = GetScreenHeight()-bc_height;

    ImageObject* bb = Image_Init(NULL);

    bc->color = (Color){255, 255, 255, 180};

    Image_Copy(bc, bb, true);

    bc->destination = (Rectangle){0, startPointC, GetScreenWidth()*2, bc_height};
    bb->destination = (Rectangle){0, startPointB, GetScreenWidth()*2, bc_height};

    Image_FlipHPro(bc);

    ImageObject* bp = Image_Init("sprites/parallax.png");
    bp->source = (Rectangle){2560.0f, 0, 2560.0f, 640.0f};

    float bp_height = ((float)GetScreenWidth()*2*bp->source.height)/bp->source.width;;

    bp->destination = (Rectangle){0, GetScreenHeight()-(bp_height/2), GetScreenWidth()*2, bp_height};

    int randSpeed1 = (rand() % 2) + 1;
    int randSpeed2 = (rand() % 2) + 1;
    int randSpeed3 = (rand() % 20) + 10;

    int minSpeed = 10;

    ImageObject* nv = Image_Init("sprites/parallax.png");
    nv->source = (Rectangle){0, 640.0f, 2560.0f, 640.0f};

    float nv_height = ((float)GetScreenWidth()*2*nv->source.height)/nv->source.width;

    nv->destination = (Rectangle){0, finalPointB-nv_height/4, GetScreenWidth()*2, nv_height};

    ImageObject* nn = Image_Init(NULL);

    Image_Copy(nv, nn, true);

    nn->destination.y = finalPointC-nv_height/4;

    bc->color.a = 0;
    bp->color.a = 0;
    bb->color.a = 0;
    nv->color.a = 0;
    nn->color.a = 0;

    bool introduction = true;
    float elapsed = 0;
    float elapsedN = 0;

    Player_setName(player, "Player");
    Player_SetSpriteSheet(player, "sprites/PlayerAnim/PlayerSpriteSheet2.png");
    Player_setCharacter(player, 0.0f);

    Player_setControl(player, false);

    Player_setDisplay(player, (Vector2){0.0f, 0.0f});
    Player_setDeltaX(player, 64.0f);
    Player_setDeltaY(player, 64.0f);
    Player_setAnimationFrames(player, (FramesAnimation){true, 0.0f, 0.0f, 1, 0, 15, 0, 11, 11});

    Player_SetSourceRec(player, (Rectangle){0.0f, 0.0f, 64.0f, 64.0f});

    Rectangle playerDestRec = (Rectangle){-240.0f, (float)(GetScreenHeight()/2 - 240/2), 240.0f, 240.0f};

    Player_SetDestRec(player, playerDestRec);
    Player_MoveTo(player, (Vector2){0.0f - playerDestRec.width, 350.0f - playerDestRec.height}, 0.0f);

    Player_setName(enemy, "Enemy");
    Player_SetSpriteSheet(enemy, "sprites/skeletonAnim/SkeletonEnemy.png");
    Player_setCharacter(enemy, 0.0f);

    Player_setControl(enemy, false);

    Player_setDisplay(enemy, (Vector2){0.0f, 0.0f});
    Player_setDeltaX(enemy, 64.0f);
    Player_setDeltaY(enemy, 64.0f);
    Player_setAnimationFrames(enemy, (FramesAnimation){true, 0.0f, 0.0f, 1, 0, 4, 0, 7, 7});

    Player_SetSourceRec(enemy, (Rectangle){0.0f, 0.0f, -64.0f, 64.0f});
    
    Rectangle enemyDestRec = (Rectangle){0, 0, 240, 240};

    Player_SetDestRec(enemy, enemyDestRec);
    Player_MoveTo(enemy, (Vector2){GetScreenWidth(), 350.0f - enemyDestRec.height}, 0.0f);

    ChangePositionFunction(Player_getAnimationPosition(player), linearFunction);

    // BACKGROUND

    ImageObject* background = Image_Init("sprites/battleBackground.png");
    background->destination = (Rectangle){0, 0, GetScreenWidth(), (background->image.height * GetScreenWidth())/background->image.width};

    // END BACKGROUND

    // BUTTONS

    ImageObject* icons = Image_Init("sprites/icons.png");

    ImageObject* healthSprite = Image_Init(NULL);
    ImageObject* atkSprite = Image_Init(NULL);
    ImageObject* defSprite = Image_Init(NULL);
    ImageObject* potionSprite = Image_Init(NULL);
    ImageObject* runSprite = Image_Init(NULL);

    Image_Copy(icons, healthSprite, true);
    Image_Copy(icons, atkSprite, true);
    Image_Copy(icons, defSprite, true);
    Image_Copy(icons, potionSprite, true);
    Image_Copy(icons, runSprite, true);

    healthSprite->source = (Rectangle){0, 64*0, 64, 64};
    atkSprite->source = (Rectangle){0, 64*1, 64, 64};
    defSprite->source = (Rectangle){0, 64*2, 64, 64};
    potionSprite->source = (Rectangle){0, 64*3, 64, 64};
    runSprite->source = (Rectangle){0, 64*4, 64, 64};

    int qButtons = 4;
    float posP = GetScreenWidth()/(qButtons*2);

    Button* atk = Button_Init("ATK");
    Button_FitSizeToText(atk, 35, (Vector2){3, 3});
    Button_Pos(atk, (Vector2){posP*1 - atk->width/2, finalPointB - atk->height * 1.1f});
    Button_SetPattern(atk, (Pattern){WHITE, WHITE, BLACK, RED, (Color){0,0,0,120}});

    Button_LoadIcon(atk, atkSprite, true);

    Button* def = Button_Init("DEF");
    Button_FitSizeToText(def, 35, (Vector2){3, 3});
    Button_Pos(def, (Vector2){posP*3 - def->width/2, finalPointB- def->height * 1.1f});
    Button_SetPattern(def, (Pattern){WHITE, WHITE, BLACK, BLUE, (Color){0,0,0,120}});

    Button_LoadIcon(def, defSprite, true);

    Button* item = Button_Init("BAG");
    Button_FitSizeToText(item, 35, (Vector2){3, 3});
    Button_Pos(item, (Vector2){posP*5 - item->width/2, finalPointB - item->height * 1.1f});
    Button_SetPattern(item, (Pattern){WHITE, WHITE, BLACK, RED, (Color){0,0,0,120}});

    Button_LoadIcon(item, potionSprite, true);

    Button* run = Button_Init("RUN");
    Button_FitSizeToText(run, 35, (Vector2){3, 3});
    Button_Pos(run, (Vector2){posP*7 - run->width/2, finalPointB - run->height * 1.1f});
    Button_SetPattern(run, (Pattern){WHITE, WHITE, BLACK, PURPLE, (Color){0,0,0,120}});

    Button_LoadIcon(run, runSprite, true);

    Button* showStats = Button_Init("Show Stats");
    Button_FitSizeToText(showStats, 17, (Vector2){8, 8});
    Button_Pos(showStats, (Vector2){posP*7 - showStats->width/2, bc->destination.y + bc->destination.height});
    Button_SetPattern(showStats, (Pattern){WHITE, WHITE, WHITE, BLACK, (Color){0,0,0,120}});

    // END BUTTONS

    float geralHeight = atk->y + atk->height + atk->padding.y;

    bool statsShowing = false;
    bool statsFading = false;
    bool introductionStats = false;
    bool permShowing = false;
    int statsOpacity = 0;

    Stats playerStats = Player_getStats(player);

    Button* health = Button_Init(TextFormat("%.0f/%.0f", playerStats.health, playerStats.maxHealth));
    Button* atkStat = Button_Init(TextFormat("%.1f", playerStats.attack));
    Button* defStat = Button_Init(TextFormat("%.1f%%", playerStats.defense * 100));
    Button* itemsQntUtils = Button_Init(TextFormat("%d", listaTamanho(Player_getInventarioUtils(player))));
    //Button* itemsQntWep = Button_Init(TextFormat("%d", listaTamanho(Player_getInventarioWeapons(player))));
    Button* runStat = Button_Init(TextFormat("%.1f%%", playerStats.evasionRate * 100));

    Stats enemyStats = Player_getStats(enemy);

    Button* healthEnemy = Button_Init(TextFormat("%.0f/%.0f", enemyStats.health, enemyStats.maxHealth));
    Button* atkStatEnemy = Button_Init(TextFormat("%.1f", enemyStats.attack));
    Button* defStatEnemy = Button_Init(TextFormat("%.1f%%", enemyStats.defense));

    Lista* statsButtons = criaLista();

    inserirFim(statsButtons, health);
    inserirFim(statsButtons, atkStat);
    inserirFim(statsButtons, defStat);
    inserirFim(statsButtons, itemsQntUtils);
    //inserirFim(statsButtons, itemsQntWep);
    inserirFim(statsButtons, runStat);

    inserirFim(statsButtons, healthEnemy);
    inserirFim(statsButtons, atkStatEnemy);
    inserirFim(statsButtons, defStatEnemy);

    percorrerLista(statsButtons, formatButtons);

    Button_LoadIcon(health, healthSprite, false);
    Button_LoadIcon(atkStat, atkSprite, false);
    Button_LoadIcon(defStat, defSprite, true);
    Button_LoadIcon(itemsQntUtils, potionSprite, false);
    //Button_LoadIcon(itemsQntWep, atkSprite, false);
    Button_LoadIcon(runStat, runSprite, true); 

    Button_LoadIcon(healthEnemy, healthSprite, true); 
    Button_LoadIcon(atkStatEnemy, atkSprite, true); 
    Button_LoadIcon(defStatEnemy, defSprite, true); 

    Button_Pos(health, (Vector2){atk->x + atk->width - (health->width), geralHeight});
    Button_Pos(atkStat, (Vector2){atk->x, geralHeight});
    Button_Pos(defStat, (Vector2){def->x + def->width/2 - defStat->width/2, geralHeight});
    Button_Pos(itemsQntUtils, (Vector2){item->x, geralHeight});
    //Button_Pos(itemsQntWep, (Vector2){item->x + item->width - (itemsQntWep->width), geralHeight});
    Button_Pos(runStat, (Vector2){run->x + run->width/2 - runStat->width/2, geralHeight});

    float enemyStatsHeight = showStats->y + showStats->height + showStats->padding.y/2;

    Button_Pos(healthEnemy, (Vector2){showStats->x + showStats->width/2 - healthEnemy->width/2, enemyStatsHeight + healthEnemy->padding.y});
    Button_Pos(atkStatEnemy, (Vector2){showStats->x + showStats->width/2 - atkStatEnemy->width/2, enemyStatsHeight + healthEnemy->padding.y + healthEnemy->height + atkStatEnemy->padding.y});
    Button_Pos(defStatEnemy, (Vector2){showStats->x + showStats->width/2 - defStatEnemy->width/2, enemyStatsHeight + healthEnemy->padding.y + healthEnemy->height + atkStatEnemy->padding.y + atkStatEnemy->height + atkStatEnemy->padding.y});

    healthEnemy->padding.x = showStats->width + showStats->padding.x - healthEnemy->width;
    atkStatEnemy->padding.x = showStats->width + showStats->padding.x - atkStatEnemy->width;
    defStatEnemy->padding.x = showStats->width + showStats->padding.x - defStatEnemy->width;

    Arvore* enemyChoice = criaFolha(isLife50Void, enemy);
    inserirEsqArvore(enemyChoice, isLife30Void, player);
    inserirDirArvore(enemyChoice, attackVoid, enemy);
    inserirEsqArvore(enemyChoice->dir, attackVoid, enemy);

    bool switchTurns = false;
    Turn whoseTurn = {false, false, true, false};

    bool cameraZoomIn = false;

    /////

    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.target = (Vector2){0, 0};

    /////

    ImageObject* inventoryBackground = Image_Init("sprites/inventoryBackground.png");

    float newWidth = GetScreenWidth()*0.92f;
    float newHeight = (newWidth*inventoryBackground->source.height)/inventoryBackground->source.width;

    Rectangle backgorundDest = {GetScreenWidth()/2 - newWidth/2, GetScreenHeight(), newWidth, newHeight};

    inventoryBackground->destination = backgorundDest;

    bool inventoryShowing = false;
    Turn inventoryAnimation = {false, false, false, false};

    Turn enemyAnimState = {false, false, false, false};
    Turn playerAnimState = {false, false, false, false};
    bool loopPlayer = false;
    bool loopEnemy = false;
    bool loopAnimPlayer = false;
    bool finishLoopPlayer = false;

    int framesToLoopPlayer = 0;

    Player_setLocked(player, true);

    healedOrHurt enemyTookDamage = {false, false};
    healedOrHurt playerTookDamage = {false, false};

    bool doZoom = false;
    bool enemyHasAttacked = false;

    bool enemyTookAction = false;
    bool playerTookAction = false;

    SpriteSheet* confetti = SpriteSheet_Init("sprites/Confetti.png", (FramesAnimation){false, 0.0f, 0.0f, 1, 0, 63, 0, 24, 24});

    SpriteSheet_SetSourceRec(confetti, (Rectangle){0, 0, 150.0f, 120.0f});
    SpriteSheet_SetDisplay(confetti, (Vector2){0.0f, 0.0f});

    float confettiWidth = (GetScreenHeight()*150.0f)/120.0f;

    SpriteSheet_SetDestRec(confetti, (Rectangle){GetScreenWidth()/2 - confettiWidth/2, 0 - GetScreenHeight()*1/8, confettiWidth, GetScreenHeight()});
    SpriteSheet_SetDelta(confetti, (Vector2){150.0f, 0.0f});

    Turn confettiAnimState = {false, false, false, false};

    TextObject* textP = Text_Init("");
    TextObject* textE = Text_Init("");

    float worldSpeed = 1.0f;

    ImageObject* healthBarSprite = Image_Init("sprites/healthBarLayout.png");
    ImageObject* healthBarFiller = Image_Init("sprites/healthBarBlend.png");

    ImageObject* healthBarSpriteP = Image_Init("sprites/healthBarLayout.png");
    ImageObject* healthBarFillerP = Image_Init("sprites/healthBarBlend.png");

    float SpriteNewW = enemyDestRec.width*4/5;
    float SpriteNewH = (SpriteNewW*healthBarSprite->source.height)/healthBarSprite->source.width;

    bool introduction2 = true;

    Player_MoveTo(enemy, (Vector2){(GetScreenWidth() - 130.0f) - enemyDestRec.width/2, playerDestRec.y + playerDestRec.height - enemyDestRec.height + (background->destination.height - GetScreenHeight())}, 0.0f);
    Player_MoveTo(player, (Vector2){130.0f - playerDestRec.width/2, 350.0f - playerDestRec.height + (background->destination.height - GetScreenHeight())}, 0.0f);

    Color overlay = {255, 0, 0, 0};

    Button* toggleHealthBar = Button_Init("Toggle Health Bar");

    Button_FitSizeToText(toggleHealthBar, showStats->fontsize, (Vector2){3, 3});

    Button_Pos(toggleHealthBar, (Vector2){showStats->x - toggleHealthBar->width * 5/4, showStats->y});

    toggleHealthBar->colors = showStats->colors;

    bool showHealthBar = true;

    float totalElapsed = 0;

    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousePos = GetMousePosition();
        resources.cursor->x = mousePos.x;
        resources.cursor->y = mousePos.y;

        totalElapsed += deltaTime;

        playerDestRec = Player_getDestRec(player);
        enemyDestRec = Player_getDestRec(enemy);

        Decision playerDecision = Player_getAction(player);
        Decision enemyDecision = Player_getAction(enemy);
        playerStats = Player_getStats(player);
        enemyStats = Player_getStats(enemy);

        Player_setAnimationFramesSpeed(player, (float)Player_getAnimationFramesBaseSpeed(player)*worldSpeed);
        Player_setAnimationFramesSpeed(enemy, (float)Player_getAnimationFramesBaseSpeed(enemy)*worldSpeed);

        if(Wait(0.3f)){
            randSpeed1 = (randSpeed1 <= minSpeed) ? minSpeed : randSpeed1 - 1;
            randSpeed2 = (randSpeed2 <= minSpeed) ? minSpeed : randSpeed2 - 1;
        }

        Player_UpdatePosition(player, deltaTime);
        Player_setAnimationFramesAnimating(player, true);
        playerAnimState = Player_UpdateSpriteExt(player, loopAnimPlayer, loopPlayer, framesToLoopPlayer, finishLoopPlayer);

        Player_UpdatePosition(enemy, deltaTime);
        Player_setAnimationFramesAnimating(enemy, true);
        enemyAnimState = Player_UpdateSprite(enemy, false, loopEnemy);

        confettiAnimState = SpriteSheet_UpdateSprite(confetti, false, false);

        if(Player_getAction(enemy) == DEAD && enemyAnimState.animationEnd){
            SpriteSheet_setAnimationFramesAnimating(confetti, true);
        }

        if(enemyTookDamage.hurt){
            Player_setAction(enemy, HURT);
        }
        else if(enemyAnimState.animationEnd){
            if(enemyTookAction && enemyAnimState.animationEnd){
                if(LOG) printf("\nEnemy switched turns..");
                switchTurns = true;
                enemyTookAction = false;
            }

            if(Player_getAnimationPositionAnimating(enemy)){
                Player_setAction(enemy, WALK);
            } else{
                Player_setAction(enemy, IDLE);
            }
            if(enemyStats.health <= 0){
                Player_setAction(enemy, DEAD);
            }
            if(!whoseTurn.animationBool && whoseTurn.animationEnd && enemyDecision != DEAD){
                percorrerArvore(enemyChoice);
                enemyTookAction = true;
            }
        }

        if(playerTookDamage.hurt && !playerStats.defending){
            Player_setAction(player, HURT);
        }
        else if(playerAnimState.animationEnd){
            if(playerTookAction && playerAnimState.animationEnd && whoseTurn.animationBool){
                if(LOG) printf("\nPlayer Switched Turns...");
                switchTurns = true;
            }
            if(Player_getAnimationPositionAnimating(player)){
                Player_setAction(player, WALK);
            } else{
                Player_setAction(player, IDLE);
            }
            if(playerStats.health <= 0){
                Player_setAction(player, DEAD);
            }

            if(playerStats.defending){
                Player_setAction(player, DEFEND);
            }
        }

        switch(enemyDecision){
            case ATTACK:
                Player_ChangeSprite(enemy, 12, 0);
                if(!enemyHasAttacked){
                    Player_TakeDamage(player, enemyStats.attack);
                    enemyHasAttacked = true;
                }
                loopEnemy = false;
                break;
            case HURT:
                Player_ChangeSprite(enemy, 4, 4);
                loopEnemy = false;
                break;
            case DEAD:
                Player_ChangeSprite(enemy, 13, 1);
                loopEnemy = false;
                break;
            case WALK:
                Player_ChangeSprite(enemy, 12, 2);
                loopEnemy = true;
                break;
            case IDLE:
                Player_ChangeSprite(enemy, 4, 3);
                loopEnemy = true;
                break;
            case HEAL:
                if(!enemyHasAttacked){
                    Player_getHealing(enemy, 2.0f);
                    enemyHasAttacked = true;
                }
                break;
            default: // Run IDLE
                Player_ChangeSprite(enemy, 4, 3);
                loopEnemy = true;        
                break;
        }

        switch(playerDecision){
            case ATTACK:
                Player_ChangeSprite(player, 15, 0);
                if(!playerTookAction){
                    playerTookAction = true;
                    Player_TakeDamage(enemy, playerStats.attack);
                    Player_setAction(enemy, HURT);
                }
                loopPlayer = false;
                loopAnimPlayer = false;
                break;
            case HURT:
                Player_ChangeSprite(player, 7, 4);
                loopPlayer = false;
                loopAnimPlayer = false;
                break;
            case DEAD:
                Player_ChangeSprite(player, 15, 1);
                loopPlayer = false;
                loopAnimPlayer = false;
                break;
            case WALK:
                Player_ChangeSprite(player, 8, 2);
                loopPlayer = true;
                loopAnimPlayer = false;
                break;
            case IDLE:
                Player_ChangeSprite(player, 15, 3);
                loopPlayer = true;
                loopAnimPlayer = false;
                break;
            case DEFEND:
                Player_ChangeSprite(player, 7, 4);
                loopPlayer = true;
                loopAnimPlayer = true;
                framesToLoopPlayer = 1;
                break;
            case HEAL:
                if(!playerTookAction){
                    Player_getHealing(player, 2.0f);
                    playerTookAction = true;
                }
                break;
            default: // Run IDLE
                Player_ChangeSprite(player, 15, 3);
                loopPlayer = true;
                loopAnimPlayer = false;
                break;
        }

        bc->destination.x = (bc->destination.x - randSpeed1*worldSpeed <= -bc->destination.width/2.0f) ? 0 : bc->destination.x - randSpeed1*worldSpeed;
        bb->destination.x = (bb->destination.x - randSpeed2*worldSpeed <= -bb->destination.width/2.0f) ? 0 : bb->destination.x - randSpeed2*worldSpeed;
        nv->destination.x = (nv->destination.x - randSpeed3*worldSpeed <= -nv->destination.width/2.0f) ? 0 : nv->destination.x - randSpeed3*worldSpeed;
        nn->destination.x = nv->destination.x;
        bp->destination.x = bc->destination.x;

        if(Button_IsPressed(atk, mousePos) && whoseTurn.animationBool && !playerTookAction){
            Player_setAction(player, ATTACK);
        }

        if(Button_IsPressed(def, mousePos) && whoseTurn.animationBool && !playerTookAction){
            Player_setDefense(player, true);
            playerTookAction = true;
        }

        if(Button_IsPressed(item, mousePos) && whoseTurn.animationBool){
            inventoryShowing = true;
        }

        if(inventoryAnimation.animationBool && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mousePos, inventoryBackground->destination)){
            inventoryShowing = true;
        }

        if(playerDecision == DEAD || enemyDecision == DEAD){
            switchTurns = false;
        }

        if(confettiAnimState.animationEnd){
            Player_setStats(resources.player, Player_getStats(player));
            return FREE;
        }

        if(Button_IsPressed(toggleHealthBar, mousePos)){
            showHealthBar = !showHealthBar;
        }

        percorrerListaRel(Player_getInventarioUtils(player), deleteUsedItems, player);

        BeginDrawing();
        BeginMode2D(camera);
            ClearBackground(BLACK);
            if(introduction2 && totalElapsed/1.0f > 1.0f){
                elapsedN += deltaTime;
                
                float pg = elapsedN / 2.0f;
                float t = -pow(pg-1, 4.0f) + 1;

                camera.target.y = Slerp(0, (background->destination.height - GetScreenHeight()), t);
                camera.target.x = 0;

                if(pg > 1.0f){
                    introduction2 = false;
                    elapsedN = 0;
                }
            }
            
            Image_DrawPro(background);

            healthBarSprite->destination = (Rectangle){enemyDestRec.x + enemyDestRec.width/2 - SpriteNewW/2, enemyDestRec.y + enemyDestRec.height*1/3, SpriteNewW, SpriteNewH};
            enemyTookDamage = healthBar(healthBarSprite, healthBarFiller, (Vector2){healthBarSprite->destination.x, healthBarSprite->destination.y}, enemyStats.health, enemyStats.maxHealth, deltaTime);

            healthBarSpriteP->destination = (Rectangle){playerDestRec.x + playerDestRec.width/2 - SpriteNewW/2, playerDestRec.y + playerDestRec.height*1/3, SpriteNewW, SpriteNewH};
            playerTookDamage = healthBarP(healthBarSpriteP, healthBarFillerP, (Vector2){healthBarSpriteP->destination.x, healthBarSpriteP->destination.y}, playerStats.health, playerStats.maxHealth, deltaTime);

            if(showHealthBar){
                Image_DrawPro(healthBarFiller);
                Image_DrawPro(healthBarSprite);
                Image_DrawPro(healthBarFillerP);
                Image_DrawPro(healthBarSpriteP);
            }

            if(IsKeyPressed(KEY_ESCAPE)){
                int retural = menuOpen();
                if(retural == 3){
                    CloseWindow();
                }
            }

            if(!introduction2 && introduction){
                elapsedN += deltaTime;
                
                float pg = elapsedN / 2.0f;
                float t = -pow(pg-1, 4.0f) + 1;

                //bc->destination.y = Slerp(startPointC, finalPointC, t);
                //bb->destination.y = Slerp(startPointB, finalPointB, t);

                bc->color.a = Slerp(0, 120, t);
                bb->color.a = Slerp(0, 120, t);
                bp->color.a = Slerp(0, 255, t);
                nv->color.a = Slerp(0, 255, t);
                nn->color.a = Slerp(0, 255, t);

                if(pg > 1.0f){
                    introduction = false;
                    elapsedN = 0;
                }
            }

            Player_Draw(player);
            Player_Draw(enemy);

            EndMode2D();

            Image_DrawPro(bb);
            Image_DrawPro(bc);
            Image_DrawPro(nv);
            Image_DrawPro(nn);
            Image_DrawPro(bp);

            if(Button_Hovering(showStats, mousePos)){
                if(!statsShowing) introductionStats = true;
                statsShowing = true;
                statsFading = false;
            }
            else if(statsShowing){
                statsFading = true;
                statsShowing = false;
            }

            if(Button_IsPressed(showStats, mousePos)){
                permShowing = !permShowing;
                statsOpacity = 255;
            }

            if(introductionStats && !permShowing){
                elapsed += deltaTime;
                
                float pg = elapsed / 1.3f;
                float t = (float)(cos(2.0f * 3.14f * (0.5f + (double)pg)) + 1.0f)/2.0f;

                statsOpacity = Slerp(0, 255, t);

                if(pg > 1.0f){
                    if(statsFading){
                        introductionStats = false;
                        statsFading = false;
                    }
                    elapsed = 0;
                }
            }

            LinkButtonColors(health, atk, statsOpacity, BLACK);
            LinkButtonColors(atkStat, atk, statsOpacity, BLACK);
            LinkButtonColors(defStat, def, statsOpacity, BLACK);
            LinkButtonColors(itemsQntUtils, item, statsOpacity, BLACK);
            //LinkButtonColors(itemsQntWep, item, statsOpacity, BLACK);
            LinkButtonColors(runStat, run, statsOpacity, BLACK);

            LinkButtonColors(healthEnemy, showStats, statsOpacity, WHITE);
            LinkButtonColors(atkStatEnemy, showStats, statsOpacity, WHITE);
            LinkButtonColors(defStatEnemy, showStats, statsOpacity, WHITE);

            Button_SetText(health, TextFormat("%.0f/%.0f", playerStats.health, playerStats.maxHealth));
            Button_SetText(healthEnemy, TextFormat("%.0f/%.0f", enemyStats.health, enemyStats.maxHealth));

            percorrerLista(statsButtons, Button_DrawIconVoid);

            Button_DrawIcon(atk);
            Button_DrawIcon(def);
            Button_DrawIcon(item);
            Button_DrawIcon(run);
            
            Button_Draw(showStats);
            Button_Draw(toggleHealthBar);

            if(whoseTurn.animationBegin) cameraZoomIn = !cameraZoomIn;

            if(switchTurns){
                whoseTurn = changeTurn(deltaTime);

                if(whoseTurn.animating) worldSpeed = 0.6f;
                else worldSpeed = 1.0f;

                if(whoseTurn.animationEnd){
                    switchTurns = false;
                    enemyHasAttacked = false;

                    if(whoseTurn.animationBool){
                        playerTookAction = false;
                        Player_setDefense(player, false);
                        finishLoopPlayer = true;
                    }
                    else finishLoopPlayer = false;
                }
            }

            if(cameraZoomIn && doZoom){            
                bool w = cameraAnimation(&camera, (Vector2){GetScreenWidth()/2, GetScreenHeight()/2}, (Vector2){enemyDestRec.x + enemyDestRec.width/2, enemyDestRec.y + enemyDestRec.height/2}, deltaTime);
                if(w){
                    cameraZoomIn = false;
                }
            }

            if(IsKeyPressed(KEY_TAB) && !inventoryAnimation.animating){
                inventoryShowing = !inventoryShowing;
            }

            if(inventoryShowing){
                inventoryAnimation = openInventory(inventoryBackground, deltaTime);
                
                if(inventoryAnimation.animationEnd){
                    inventoryShowing = false;

                    // ACTIVATE/DEACTIVATE BUTTONS BASED ON INVENTORY SHOWING
                    Button_SwitchActive(atk);
                    Button_SwitchActive(def);
                    Button_SwitchActive(item);
                    Button_SwitchActive(run);
                }
            }

            Rectangle posHP = {healthBarSpriteP->destination.x + healthBarSpriteP->destination.width*1.1f, healthBarSpriteP->destination.y - 3, 0, healthBarSpriteP->destination.height};
            Rectangle posHE = {healthBarSprite->destination.x + healthBarSprite->destination.width*1.1f, healthBarSprite->destination.y - 3, 0, healthBarSprite->destination.height};

            LogHealthP(playerStats.health, textP, posHP, deltaTime);
            LogHealthE(enemyStats.health, textE, posHE, deltaTime);

            SpriteSheet_Draw(confetti);

            Image_DrawPro(inventoryBackground);
            imprimirListaRelRel(Player_getInventarioUtils(player), inventoryBackground, player, imprimirItensInventario);

            imprimirListaRel(Player_getInventarioUtils(player), player, imprimirItensDescInventario);

            // PARAMETRIZAÇÃO DO INVENTARIO
            //percorrerLista(Player_getInventario(player), imprimirInventario);
            //

            Image_Draw(resources.cursor);

            if(playerTookDamage.hurt) overlay.a = 120;

            if(overlay.a > 0) overlay.a -= 3;

            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), overlay);
            Image_DrawPro(resources.grainOverlay);

        EndDrawing();
    }
    return 0;
}

GAMESTATE telaJogo(Resources resources){
    Player* player = resources.player;
    Player* enemy = resources.enemy;

    float squaresize = resources.squareSize;

    Lista* effects = criaLista();
    Lista* paredes = criaLista();
    Lista* chao = criaLista();
    itens = criaLista();
    inimigos = criaLista();
    traps = criaLista();

    ImageObject* parede = Image_Init("sprites/white.png");
    ImageObject* borda = Image_Init("sprites/white.png");
    ImageObject* chaoTileSet = Image_Init("sprites/chaoTileSet.png");

    ImageObject* crosshair = Image_Init("sprites/white.png");

    ImageObject* paredesTileSet = Image_Init("sprites/paredes.png");

    ImageObject* spritesheetItems = Image_Init("sprites/Itens/Items.png");

    ImageObject* misteryBox = Image_Init(NULL);
    ImageObject* incense = Image_Init(NULL);
    ImageObject* mimic = Image_Init(NULL);
    ImageObject* heal = Image_Init(NULL);
    ImageObject* chest = Image_Init(NULL);
    ImageObject* warning = Image_Init(NULL);
    ImageObject* trap = Image_Init(NULL);

    Image_CopyImage(incense, spritesheetItems);
    Image_CopyImage(mimic, spritesheetItems);
    Image_CopyImage(heal, spritesheetItems);
    Image_CopyImage(chest, spritesheetItems);
    Image_CopyImage(warning, spritesheetItems);

    incense->source = (Rectangle){0, 64.0f*2.0f, 64.0f, 64.0f};
    mimic->source = (Rectangle){0, 64.0f*4.0f, 64.0f, 64.0f};
    heal->source = (Rectangle){0, 64.0f*1.0f, 64.0f, 64.0f};
    chest->source = (Rectangle){0, 64.0f*3.0f, 64.0f, 64.0f};

    Image_Copy(spritesheetItems, warning, true);
    warning->source = (Rectangle){0, 64.0f*5.0f, 64.0f, 64.0f};
    warning->destination = (Rectangle){-squaresize, -squaresize, squaresize*0.8f, squaresize*0.8f};

    Image_Copy(spritesheetItems, misteryBox, true);
    misteryBox->source = (Rectangle){0, 0, 64.0f, 64.0f};
    misteryBox->destination = (Rectangle){-squaresize, -squaresize, squaresize*0.8f, squaresize*0.8f};

    Image_Copy(spritesheetItems, trap, true);
    trap->source = (Rectangle){0, 64.0f*6.0f, 64.0f, 64.0f};
    trap->destination = (Rectangle){-squaresize, -squaresize, squaresize*0.8f, squaresize*0.8f};

    paredesTileSet->source = (Rectangle){0, 0, 16, 16};
    paredesTileSet->destination = (Rectangle){0, 0, squaresize/2, squaresize/2};

    Image_Resize(parede, squaresize, squaresize);
    Image_Resize(borda, squaresize, squaresize);
    Image_Resize(chaoTileSet, squaresize, squaresize);
    Image_Resize(crosshair, squaresize, squaresize);

    crosshair->color = (Color){255, 255, 255, 30};
    parede->color = (Color){0, 0, 30, 150};
    borda->color = (Color){80, 0, 0, 255};

    int** mapa = resources.mapa;

    Player_SetMoveSet(player, (MoveSet){KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT});
    Player_SetStepSize(player, squaresize);

    Player_SetStepSize(enemy, squaresize);
    Player_ChangeCharacter(enemy);

    ImageObject* grama = Image_Init("sprites/grass.png");
    grama->color = (Color){255,255,255,255};

    grama->source = (Rectangle){0, 0, 32.0f, 32.0f};
    grama->destination = (Rectangle){0, 0, squaresize, squaresize};

    //////////////////////

    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = cameraInitZoom;

    //////////////////////

    bool moved = false;

    ImageObject* chestBackground = Image_Init("sprites/Chest_GUI.png");
    
    float chestBGw = GetScreenWidth()*0.8f;
    float chestBGh = (chestBGw*chestBackground->source.height)/chestBackground->source.width;
    
    chestBackground->destination = (Rectangle){GetScreenWidth()/2 - chestBGw/2, GetScreenHeight(), chestBGw, chestBGh};

    bool chestShowing = false;
    Turn chestAnimation = {false, false, false, false};

    float ratioChestWidth = (float)chestBGw/chestBackground->source.width;
    float ratioChestHeight = (float)chestBGh/chestBackground->source.height;

    Rectangle chestSprite = {chestBackground->destination.x + chestBackground->destination.width/2 - (50.0f * ratioChestWidth)/2, 22.0f * ratioChestHeight, 50.0f * ratioChestWidth, 50.0f * ratioChestHeight};

    ///////////////////////////////////////////

    ImageObject* inventoryBackground = Image_Init("sprites/inventoryBackground.png");
    
    float newWidth = GetScreenWidth()*0.92f;
    float newHeight = (newWidth*inventoryBackground->source.height)/inventoryBackground->source.width;

    Rectangle backgorundDest = {GetScreenWidth()/2 - newWidth/2, GetScreenHeight(), newWidth, newHeight};

    inventoryBackground->destination = backgorundDest;

    bool inventoryShowing = false;
    Turn inventoryAnimation = {false, false, false, false};

    ////////////////////////////////////////////

    // INICIALIZA O MAPA (INTERFACE)
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            if(mapa[i][j] == 1) Player_MoveTo(player, (Vector2){j*squaresize, i*squaresize}, 0.0f);

            CreateGround(mapa, i, j, squaresize, chao, chaoTileSet);

            // WALLS
            HandleTile(mapa, i, j, squaresize, paredes, paredesTileSet, 2);

            int grassChance = rand() % 100;

            if(mapa[i][j] == 6){ // ITEM
                ImageObject* msInstance = Image_Init(NULL);
                Image_Copy(misteryBox, msInstance, true);
                msInstance->destination.x = j*squaresize + squaresize/2 - misteryBox->destination.width/2;
                msInstance->destination.y = i*squaresize + squaresize/2 - misteryBox->destination.height/2;
                
                msInstance->save = msInstance->destination.y;

                inserirFim(itens, msInstance);

                grassChance = 100;
            }

            if(mapa[i][j] == 7){ // ENEMY
                ImageObject* enemyInstance = Image_Init(NULL);
                Image_Copy(warning, enemyInstance, true);
                enemyInstance->destination.x = j*squaresize + squaresize/2 - enemyInstance->destination.width/2;
                enemyInstance->destination.y = i*squaresize + squaresize/2 - enemyInstance->destination.height/2;
                
                enemyInstance->save = enemyInstance->destination.y;

                inserirFim(inimigos, enemyInstance);

                grassChance = 100;
            }

            if(mapa[i][j] == 5){
                ImageObject* trapInstance = Image_Init(NULL);
                Image_Copy(trap, trapInstance, true);
                trapInstance->destination.x = j*squaresize + squaresize/2 - trapInstance->destination.width/2;
                trapInstance->destination.y = i*squaresize + squaresize/2 - trapInstance->destination.height/2;
                
                trapInstance->save = trapInstance->destination.y;

                inserirFim(traps, trapInstance);

                grassChance = 100;
            }

            if(grassChance < 40 || mapa[i][j] == 2) continue;
            
            generateGrass(effects, grama, (Vector2){i, j}, squaresize);
        }
    }

    printMapa(mapa, TAM);

    bool triggerCutscene = false;

    CutsceneFunctions battleTransition = {quadraticFunction, battleTransitionF, battleTransitionR};

    ImageObject* itemPlaceHolder = Image_Init("sprites/Itens/Items.png");
    Item* itemObj = Item_Init(0, NULL);

    TextObject* itemNameText = Text_Init("Top Text");
    TextObject* itemDescText = Text_Init("Bottom Text");

    itemNameText->fontsize = 50;
    itemDescText->fontsize = 20;

    int getItemKey = KEY_E;
    int leaveItemKey = KEY_Q;

    Button* getItem = Button_Init("Pegar");
    Button* leaveItem = Button_Init(TextFormat("Deixar [%c]", leaveItemKey));

    Button_FitSizeToText(getItem, 30, (Vector2){2, 2});
    Button_FitSizeToText(leaveItem, 30, (Vector2){2, 2});

    MoveSet mapKeys = {KEY_KP_8, KEY_KP_2, KEY_KP_4, KEY_KP_6};

    bool playerCanMove = true;
    bool onWayMove = false;

    bool mimicTrigger = false;

    TextObject* fightNotification = Text_Init("!");
    fightNotification->fontsize = 40;
    fightNotification->color = RED;

    ImageObject* healthBarFillerP = Image_Init("sprites/healthBarBlend.png");
    ImageObject* healthBarSpriteP = Image_Init("sprites/healthBarLayout.png");

    float SpriteNewW = GetScreenWidth()*2/5;
    float SpriteNewH = (SpriteNewW*healthBarSpriteP->source.height)/healthBarSpriteP->source.width;

    healedOrHurt playerTookDamage = {false, false};

    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousepos = GetScreenToWorld2D(GetMousePosition(), camera);
        Vector2 mouseposW = GetMousePosition();

        resources.cursor->x = mouseposW.x;
        resources.cursor->y = mouseposW.y;

        Rectangle playerDestRec = Player_getDestRec(player);
        Stats playerStats = Player_getStats(player);

        if(!Player_getAnimationPositionAnimating(player) && !Player_getLocked(player)){
            onWayMove = false;
            Vector2 direction = {0, 0};

            if(!onWayMove){
                if(IsKeyDown(mapKeys.up)) direction = (Vector2){0, -1};
                if(IsKeyDown(mapKeys.left)) direction = (Vector2){-1, 0};
                if(IsKeyDown(mapKeys.right)) direction = (Vector2){1, 0};
                if(IsKeyDown(mapKeys.down)) direction = (Vector2){0, 1};
            }

            moved = mover(mapa, TAM, player, direction);
            if(moved && !Vector2EqualZERO(direction)){
                Player_StepTo(player, direction, true);
                onWayMove = true;
                Player_subRepelent(player, 1);
            }
            playerCanMove = moved;
        }

        bool lockPlayer = triggerCutscene || chestAnimation.animationBool || inventoryAnimation.animationBool || chestAnimation.animating || inventoryAnimation.animating;

        Player_setLocked(player, lockPlayer);

        bool altUpdateSprite = (!lockPlayer && ((IsKeyDown(mapKeys.up) || IsKeyDown(mapKeys.down) || IsKeyDown(mapKeys.left) || IsKeyDown(mapKeys.right)) || Player_getAnimationPositionAnimating(player)));

        Player_setAnimationFramesAnimating(player, altUpdateSprite);

        if(playerCanMove){
            Player_UpdatePosition(player, deltaTime);
            Player_UpdateSprite(player, true, true);

            Player_UpdateCoords(player);
        }

        Player_setAnimationFramesAnimating(player, altUpdateSprite);

        if(IsKeyPressed(KEY_T) && !inventoryAnimation.animating){ // INVENTORY UI TRIGGER
            inventoryShowing = true;

            if(!inventoryAnimation.animationBool){
                abrirInventario(player);
            }
        }
        if(IsKeyDown(KEY_D) && !Player_getAnimationPositionAnimating(player)){ // UNDO TRIGGER
            Vector2 moveBack = desfazerMovimento(mapa, TAM, player);
            Player_StepTo(player, moveBack, true);
        }
        if((collidedWithEnemy || mimicTrigger)){ // FIGHT TRIGGER
            triggerCutscene = true;
            mimicTrigger = false;
            collidedWithEnemy = false;
        }

        if(IsKeyPressed(KEY_M)){
            printMapa(mapa, TAM);
        }

        if(chestAnimation.animationBool){
            if((Button_IsPressed(getItem, mouseposW) || IsKeyPressed(getItemKey)) && !chestShowing){
                if(itemObj->id == 3){
                    if(itemObj->isMimic){
                        itemObj->sprite->source = mimic->source;
                        mimicTrigger = true;

                        Text_Set(itemNameText, "AHHH A MIMIC");
                        itemNameText->color = RED;

                        itemNameText->x = GetScreenWidth()/2 - MeasureText(itemNameText->text, itemNameText->fontsize)/2;
                    }
                    else{
                        Player_addGold(player, 30);
                    }
                }
                else{
                    Item* addInvItem = Item_Copy(itemObj);

                    addInvItem->indice = listaTamanho(Player_getInventarioUtils(player));

                    inserirFim(Player_getInventarioUtils(player), addInvItem);
                }

                chestShowing = true;
            }
            if(Button_IsPressed(leaveItem, mouseposW) || IsKeyPressed(leaveItemKey)){
                chestShowing = true;
            }
        }

        camera.target = (Vector2){playerDestRec.x + playerDestRec.width/2, playerDestRec.y + playerDestRec.height/2};

        if(IsKeyPressed(KEY_ESCAPE)){
            int returnal = menuOpen();
            if(returnal == 3){
                CloseWindow();
            }
        }

        percorrerListaRel(itens, upDownFunction, &deltaTime);
        percorrerListaRel(itens, isCollidingWithPlayer, player); // DeletedMisteryBox
        percorrerListaRel(inimigos, upDownFunction, &deltaTime);
        percorrerListaRel(inimigos, isCollidingWithPlayerEnemy, player); // CollidedWithEnemy
        percorrerListaRel(traps, upDownFunction, &deltaTime);
        percorrerListaRel(traps, isCollidingWithPlayerTrap, player); // CollidedWithEnemy

        if(deletedMisteryBox){ // ITEM UI TRIGGER
            int item = sortearItem(true);

            switch (item){
            case 1:
                Image_ChangeImage(itemPlaceHolder, heal->image);
                itemPlaceHolder->source = heal->source;

                Button_SetText(getItem, TextFormat("Pegar [%c]", getItemKey));
                break;
            case 2:
                Image_ChangeImage(itemPlaceHolder, incense->image);
                itemPlaceHolder->source = incense->source;

                Button_SetText(getItem, TextFormat("Pegar [%c]", getItemKey));
                break;
            case 3:
                Image_ChangeImage(itemPlaceHolder, chest->image);
                itemPlaceHolder->source = chest->source;

                Button_SetText(getItem, TextFormat("Abrir [%c]", getItemKey));
                break;
            }

            Button_FitSizeToText(getItem, getItem->fontsize, (Vector2){2, 2});

            itemPlaceHolder->destination = chestSprite;

            Item_ReInit(itemObj, item, itemPlaceHolder);

            Text_Set(itemNameText, getItemName(itemObj));
            Text_Set(itemDescText, getItemDescription(itemObj));

            itemNameText->x = GetScreenWidth()/2 - MeasureText(getItemName(itemObj), itemNameText->fontsize)/2;
            itemDescText->x = GetScreenWidth()/2 - MeasureText(getItemDescription(itemObj), itemDescText->fontsize)/2;

            chestShowing = true;

            deletedMisteryBox = false;
        }

        percorrerListaRel(Player_getInventarioUtils(player), deleteUsedItems, player);

        BeginDrawing();
            BeginMode2D(camera);
            int posX = mousepos.x / squaresize;
            int posY = mousepos.y / squaresize;

            ClearBackground(BLACK);

            if(IsKeyPressed(KEY_SPACE)){
                Player_ChangeCharacter(player);
            }

            imprimirLista(chao, imprimirImageObject);
            imprimirLista(paredes, imprimirImageObjectPro);
            imprimirListaRel(effects, player, imprimirImageObjectProBH);

            imprimirLista(itens, imprimirImageObjectPro);
            imprimirLista(inimigos, imprimirImageObjectPro);
            imprimirLista(traps, imprimirImageObjectPro);

            Player_Draw(player);

            if(triggerCutscene){
                Text_Pos(fightNotification, (Vector2){playerDestRec.x + playerDestRec.width/2 - MeasureText(fightNotification->text, fightNotification->fontsize)/2, playerDestRec.y - fightNotification->fontsize});

                Text_DrawS(fightNotification);

                if(cutsceneListener(triggerCutscene, 0.5f, deltaTime, battleTransition)){
                    Text_Pos(fightNotification, (Vector2){GetScreenWidth(), GetScreenHeight()});
                    triggerCutscene = false;
                    EndMode2D();
                    EndDrawing();
                    *resources.playerCoordsSave = Player_GetCoords(player);
                    return BATTLE;
                }
            }

            imprimirListaRel(effects, player, imprimirImageObjectProFW);

            crosshair->x = posX * squaresize;
            crosshair->y = posY * squaresize;
            Image_Draw(crosshair);

            // DARKEN FUNC

            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 210});

            drawLightSource((Vector2){playerDestRec.x + playerDestRec.width/2, playerDestRec.y + playerDestRec.height/2}, squaresize*4, 80.0f);

            EndMode2D();

            if(inventoryShowing){
                inventoryAnimation = openInventory(inventoryBackground, deltaTime);
                
                if(inventoryAnimation.animationEnd) inventoryShowing = false;
            }

            if(chestShowing){
                itemPlaceHolder->destination.y = chestBackground->destination.y + chestSprite.y;
                itemPlaceHolder->save = itemPlaceHolder->destination.y;
                chestAnimation = openChest(chestBackground, deltaTime);

                if(chestAnimation.animationEnd) chestShowing = false;
            }

            Image_DrawPro(inventoryBackground);
            imprimirListaRelRel(Player_getInventarioUtils(player), inventoryBackground, player, imprimirItensInventario);
            imprimirListaRel(Player_getInventarioUtils(player), player, imprimirItensDescInventario);
            
            //Image_DrawPro(chestBackground);
            //Image_DrawPro(itemPlaceHolder);
            Image_DrawPro(itemObj->sprite);

            upDownFunction(itemObj->sprite, &deltaTime);

            Text_Pos(itemNameText, (Vector2){itemNameText->x, chestBackground->destination.y});
            Text_Pos(itemDescText, (Vector2){itemDescText->x, chestBackground->destination.y + chestBackground->destination.height});

            Text_DrawS(itemNameText);
            Text_DrawS(itemDescText);

            Button_Pos(getItem, (Vector2){itemDescText->rectangle.x + itemDescText->rectangle.width/2 - getItem->width/2, itemDescText->y + 50});
            Button_Pos(leaveItem, (Vector2){itemDescText->rectangle.x + itemDescText->rectangle.width/2 - leaveItem->width/2, getItem->y + 50});

            Button_Draw(getItem);
            Button_Draw(leaveItem);

            //Image_DrawPro(stats);
            
            healthBarSpriteP->destination = (Rectangle){GetScreenWidth()/2 - SpriteNewW/2, GetScreenHeight() - SpriteNewH*1.2f, SpriteNewW, SpriteNewH};
            playerTookDamage = healthBarP(healthBarSpriteP, healthBarFillerP, (Vector2){healthBarSpriteP->destination.x, healthBarSpriteP->destination.y}, playerStats.health, playerStats.maxHealth, deltaTime);

            Image_DrawPro(healthBarFillerP);
            Image_DrawPro(healthBarSpriteP);

            Image_Draw(resources.cursor);
            Image_DrawPro(resources.grainOverlay);
        EndDrawing();
    }

    return 0;
}

int telaInicial(Resources resources){
    ImageObject* cursor = resources.cursor;
    ImageObject* background = Image_Init("sprites/MainScreen/background.jpg");

    Image_Fit(background);

    TextObject* titulo = Text_Init("Dungeon Crawler");
    TextObject* creditos = Text_Init("Andre Ijiri Ribeiro e Caio Sweiver de Carvalho");
    TextObject* versao = Text_Init("Trabalho 1 - Estrutura de Dados");

    titulo->fontsize = 50;

    Text_Pos(titulo, (Vector2){GetScreenWidth()/2 - MeasureText(titulo->text, titulo->fontsize)/2, GetScreenHeight()*1/3 - titulo->fontsize/2});

    creditos->fontsize = 16;

    Text_Pos(creditos, (Vector2){GetScreenWidth()/2 - MeasureText(creditos->text, creditos->fontsize)/2, titulo->y + titulo->fontsize + creditos->fontsize/2});

    Button* iniciar = Button_Init("Iniciar");
    Button* sair = Button_Init("Sair");

    Button_FitSizeToText(iniciar, 50, (Vector2){3, 3});
    Button_FitSizeToText(sair, 50, (Vector2){3, 3});

    sair->width = iniciar->width;

    Button_Pos(iniciar, (Vector2){GetScreenWidth()/2 - (iniciar->width/2 + iniciar->padding.x), GetScreenHeight()*2/3});
    Button_Pos(sair, (Vector2){GetScreenWidth()/2 - (sair->width/2 + sair->padding.x), iniciar->y + iniciar->height + iniciar->padding.y*2});

    while(!WindowShouldClose()){
        Vector2 mousePos = GetMousePosition();
        cursor->x = mousePos.x;
        cursor->y = mousePos.y;

        if(Button_IsPressed(iniciar, mousePos)){
            UnloadTexture(background->image);
            free(background);
            return INTRODUCTION;
        }
        
        BeginDrawing();
            Image_Draw(background);

            Text_DrawS(titulo);
            Text_DrawS(creditos);

            Button_Draw(iniciar);
            Button_Draw(sair);

            Image_Draw(cursor);
            Image_DrawPro(resources.grainOverlay);
        EndDrawing();
    }
    return 0;
}

GAMESTATE telaIntroducao(Resources resources){
    return FREE;
}

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Teste");
    SetTargetFPS(60);

    _chdir(GetApplicationDirectory());
    SetExitKey(KEY_A);

    float screensize = (SCREEN_WIDTH + SCREEN_HEIGHT) / 2;
    float squaresize = screensize/TAM;

    Rectangle source = {0.0f, 120.0f, 160.0f, 200.0f};
    Rectangle dest = {0, 0, squaresize, squaresize};

    Player* player = Player_Init(source, dest, "sprites/Player_spritesheet.png");
    Player* enemy = Player_Init(source, dest, "sprites/Player_spritesheet.png");
    Vector2 playerCoordsSave = {1, 1};

    int** mapa = NULL;
    criarMapa(&mapa, TAM);

    srand(time(NULL));

    populaMapa(mapa, TAM);

    HideCursor();
    ImageObject* cursor = Image_Init("sprites/cursor.png");
    Image_Resize(cursor, 32, 32);

    ImageObject* grainOverlay = Image_Init("sprites/grainOverlay.jpg");

    //float newWidth = GetScreenWidth();
    //float newHeight = (newWidth*grainOverlay->source.height)/grainOverlay->source.width;

    float newHeight = GetScreenHeight();
    float newWidth = (newHeight*grainOverlay->source.width)/grainOverlay->source.height;

    grainOverlay->destination = (Rectangle){0, 0, newWidth, newHeight};

    grainOverlay->color = (Color){255, 255, 255, 10};

    Resources resources = {player, enemy, squaresize, &playerCoordsSave, mapa, cursor, grainOverlay};

    GAMESTATE gamestate = MAINSCREEN;

    while(!WindowShouldClose()){
        switch (gamestate){
            case MAINSCREEN:
                gamestate = telaInicial(resources);
                break;
            case INTRODUCTION:
                gamestate = telaIntroducao(resources);
                break;
            case FREE:
                gamestate = telaJogo(resources);
                break;
            case BATTLE:
                gamestate = fightScreen(resources);
                break;
            case ENDSCREEN:
                break;

            default:
                break;
        }
    }

    return 0;
}
