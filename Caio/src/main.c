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

#include "Lista.h"
#include "Arvore.h"

#include "Mapa.h"

#pragma endregion "Structs"

Camera2D camera = {0};
float cameraInitZoom = TAM / 8;

#pragma region "VoidPointers"

void imprimirImageObject(const void* item) {
    const ImageObject* image = (const ImageObject*)item;
    Image_Draw((ImageObject*)image);
}

void imprimirImageObjectPro(const void* item) {
    const ImageObject* image = (const ImageObject*)item;
    Image_DrawPro((ImageObject*)image);
}

void imprimirImageObjectProBH(const void* item, const void* target) {
    const ImageObject* image = (const ImageObject*)item;
    const Player* player = (const Player*)target;

    Rectangle playerDestRec = Player_getDestRec((Player*)player);

    bool isInsidePlayerX = (image->destination.x < (playerDestRec.x + playerDestRec.width) && image->destination.x > playerDestRec.x);
    bool isInsidePlayerY = (image->destination.y < (playerDestRec.y + playerDestRec.height) && image->destination.y > (playerDestRec.y));

    bool isInsidePlayer = isInsidePlayerX && isInsidePlayerY;

    if(isInsidePlayer && (image->destination.y + image->destination.height) < (playerDestRec.y + playerDestRec.height)){
        Image_DrawPro((ImageObject*)image);
    }
    else if(!isInsidePlayer){
        imprimirImageObjectPro(image);
    }
}

void imprimirImageObjectProFW(const void* item, const void* target) {
    const ImageObject* image = (const ImageObject*)item;
    const Player* player = (const Player*)target;

    Rectangle playerDestRec = Player_getDestRec((Player*)player);

    bool isInsidePlayerX = (image->destination.x < (playerDestRec.x + playerDestRec.width) && image->destination.x > playerDestRec.x);
    bool isInsidePlayerY = (image->destination.y < (playerDestRec.y + playerDestRec.height) && image->destination.y > (playerDestRec.y));

    bool isInsidePlayer = isInsidePlayerX && isInsidePlayerY;

    if(isInsidePlayer && (image->destination.y + image->destination.height) >= (playerDestRec.y + playerDestRec.height)){
        Image_DrawPro((ImageObject*)image);
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

void ImprimirInventario(const void* item){
    for(int t = 0; t < 9; t++) DrawRectangleRec((Rectangle){slotRec.x + inventoryBackground->destination.x + slotDeltaX*t, slotRec.y + inventoryBackground->destination.y, slotRec.width, slotRec.height}, BLACK);
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

#pragma endregion "VoidPointers"

#pragma region "Funcoes Uteis"

typedef struct CutsceneFunctions{
    float (*function)(float);
    void (*target)(float);
    void (*reset)();
} CutsceneFunctions;

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

    elapsed += deltaTime;
    
    float pg = elapsed / duration;
    float t = functions.function(pg);

    functions.target(t);

    if(pg > 1.0f){
        activate = false;
        elapsed = 0;
        functions.reset();

        return true;
    }

    return false;
}

void moveRandom(Player* instance, double interval){
    if(Wait(interval)){
        Player_StepTo(instance, (Vector2){(rand() % 3) - 1, (rand() % 2) - 1}, true);
    }
}

void generateGrass(Lista* lista, ImageObject* spriteSheet, Vector2 coords, float squareSize){
    int quantGrama = rand() % 17;

    for(int k = 0; k < quantGrama; k++){
        ImageObject* gramaRand = Image_Init(NULL);

        Image_Copy(spriteSheet, gramaRand, true);
        int crand = rand() % 1;
        gramaRand->color = (Color){120 + crand, 180 + rand()%50, 120 + crand, 255};

        Rectangle sourceG = {150.0f * (rand() % 3), 0.0f, 150.0f, 150.0f};
        Rectangle destG = {0, 0, squareSize/2, squareSize/2};

        destG.x = (rand() % (int)squareSize * 0.6f) + squareSize*coords.y + squareSize*0.2f;
        destG.y = ((rand() % (int)squareSize * 0.7f) + squareSize*coords.x + destG.height*0.2f);

        gramaRand->source = sourceG;
        gramaRand->destination = destG;

        inserirFim(lista, gramaRand);
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

        return (Turn){true, false, !inventoryOpened, false};
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

typedef struct healedOrHurt{
    bool healed;
    bool hurt;
} healedOrHurt;

healedOrHurt healthBarP(ImageObject* healthBarSprite, ImageObject* healthBarFiller, Vector2 initialPos, float health, float maxHealth, float deltaTime){
    static float relativeHealth = 10; // MUDAR
    static float elapsed = 0;
    static bool hurt = false;
    static bool hurt2 = false;
    static bool healed = false;
    static Color healthBarColor = WHITE;
    static Color healthBarBorderColor = WHITE;

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
    healthBarFiller->destination.width = (healthBarSprite->destination.width * health) / maxHealth;

    Image_DrawPro(healthBarFiller);
    Image_DrawPro(healthBarSprite);

    return (healedOrHurt){stateHeal, stateHurt};
}

healedOrHurt healthBar(ImageObject* healthBarSprite, ImageObject* healthBarFiller, Vector2 initialPos, float health, float maxHealth, float deltaTime){
    static float relativeHealth = 10; // MUDAR
    static float elapsed = 0;
    static bool hurt = false;
    static bool healed = false;
    static Color healthBarColor = WHITE;
    static Color healthBarBorderColor = WHITE;

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
    healthBarFiller->destination.width = (healthBarSprite->destination.width * health) / maxHealth;

    Image_DrawPro(healthBarFiller);
    Image_DrawPro(healthBarSprite);

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

typedef enum GAMESTATE{
    MAINSCREEN,
    FREE,
    BATTLE,
    ENDSCREEN
} GAMESTATE;

typedef struct Resources{
    Player* player;
    Player* enemy;
    float squareSize;
} Resources;

GAMESTATE fightScreen(Resources resources){
    Player* player = resources.player;
    Player* enemy = resources.enemy;

    ImageObject* bc = Image_Init("sprites/parallax.png");
    bc->source = (Rectangle){0, 0, 2560.0f, 640.0f};

    float bc_height = ((float)GetScreenWidth()*bc->source.height)/bc->source.width;

    bc->destination = (Rectangle){0, 0-bc_height/2, GetScreenWidth()*2, bc_height};
    bc->color = (Color){255, 255, 255, 180};

    float startPointC = bc->destination.y;
    float startPointB = GetScreenHeight();
    
    float finalPointC = 0-bc_height/2;
    float finalPointB = GetScreenHeight()-bc_height;

    ImageObject* bb = Image_Init(NULL);

    Image_Copy(bc, bb, true);

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
    background->destination = (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()};

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

    healthSprite->source = (Rectangle){0, 210*0, 210, 210};
    atkSprite->source = (Rectangle){0, 210*1, 210, 210};
    defSprite->source = (Rectangle){0, 210*2, 210, 210};
    potionSprite->source = (Rectangle){0, 210*3, 210, 210};
    runSprite->source = (Rectangle){0, 210*4, 210, 210};

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
    Button* itemsQntWep = Button_Init(TextFormat("%d", listaTamanho(Player_getInventarioWeapons(player))));
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
    inserirFim(statsButtons, itemsQntWep);
    inserirFim(statsButtons, runStat);

    inserirFim(statsButtons, healthEnemy);
    inserirFim(statsButtons, atkStatEnemy);
    inserirFim(statsButtons, defStatEnemy);

    percorrerLista(statsButtons, formatButtons);

    Button_LoadIcon(health, healthSprite, false);
    Button_LoadIcon(atkStat, atkSprite, false);
    Button_LoadIcon(defStat, defSprite, true);
    Button_LoadIcon(itemsQntUtils, potionSprite, false);
    Button_LoadIcon(itemsQntWep, atkSprite, false);
    Button_LoadIcon(runStat, runSprite, true); 

    Button_LoadIcon(healthEnemy, healthSprite, true); 
    Button_LoadIcon(atkStatEnemy, atkSprite, true); 
    Button_LoadIcon(defStatEnemy, defSprite, true); 

    Button_Pos(health, (Vector2){atk->x + atk->width - (health->width), geralHeight});
    Button_Pos(atkStat, (Vector2){atk->x, geralHeight});
    Button_Pos(defStat, (Vector2){def->x + def->width/2 - defStat->width/2, geralHeight});
    Button_Pos(itemsQntUtils, (Vector2){item->x, geralHeight});
    Button_Pos(itemsQntWep, (Vector2){item->x + item->width - (itemsQntWep->width), geralHeight});
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

    bool think = false;
    bool cameraZoomIn = false;

    HideCursor();
    ImageObject* cursor = Image_Init("sprites/cursor.png");
    Image_Resize(cursor, 32, 32);

    /////

    camera.offset = (Vector2){GetScreenWidth()/2, GetScreenHeight()/2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.target = (Vector2){GetScreenWidth()/2, GetScreenHeight()/2};

    /////

    ImageObject* inventoryBackground = Image_Init("sprites/inventoryBackground.png");

    // float newHeight = GetScreenHeight()*0.8f;
    // float newWidth = (inventoryBackground->image.width * newHeight)/inventoryBackground->image.height;

    float newWidth = GetScreenWidth()*0.92f;
    float newHeight = (newWidth*inventoryBackground->source.height)/inventoryBackground->source.width;

    Rectangle backgorundDest = {GetScreenWidth()/2 - newWidth/2, GetScreenHeight(), newWidth, newHeight};

    inventoryBackground->destination = backgorundDest;

    bool inventoryShowing = false;
    Turn inventoryAnimation = {false, false, false, false};
    
    float ratioWidth = (float)newWidth/inventoryBackground->source.width;
    float ratioHeight = (float)newHeight/inventoryBackground->source.height;

    float slotDeltaX = 72.0f * ratioWidth;

    Rectangle slotRec = {31.0f * ratioWidth, 363.0f * ratioHeight, 64.0f * ratioWidth, 64.0f * ratioHeight};

    ImageObject* healthBarSprite = Image_Init("sprites/healthBar.png");
    ImageObject* healthBarFiller = Image_Init("sprites/healthBarBar.png");

    ImageObject* healthBarSpriteP = Image_Init("sprites/healthBar.png");
    ImageObject* healthBarFillerP = Image_Init("sprites/healthBarBar.png");

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

    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousePos = GetMousePosition();
        cursor->x = mousePos.x;
        cursor->y = mousePos.y;

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
                //Player_setAction(enemy, HEAL);
                
                percorrerArvore(enemyChoice);
                enemyTookAction = true;
            }
        }

        if(playerTookDamage.hurt && !playerStats.defending){
            Player_setAction(player, HURT);
        }
        else if(playerAnimState.animationEnd){
            if(playerTookAction && playerAnimState.animationEnd){
                switchTurns = true;
                playerTookAction = false;
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
        }

        switch(playerDecision){
            case ATTACK:
                Player_ChangeSprite(player, 15, 0);
                if(!playerTookAction){
                    Player_TakeDamage(enemy, 1.0f);
                    Player_ChangeSprite(enemy, 4, 4);
                    playerTookAction = true;
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
        }

        bc->destination.x = (bc->destination.x - randSpeed1*worldSpeed <= -bc->destination.width/2.0f) ? 0 : bc->destination.x - randSpeed1*worldSpeed;
        bb->destination.x = (bb->destination.x - randSpeed2*worldSpeed <= -bb->destination.width/2.0f) ? 0 : bb->destination.x - randSpeed2*worldSpeed;
        nv->destination.x = (nv->destination.x - randSpeed3*worldSpeed <= -nv->destination.width/2.0f) ? 0 : nv->destination.x - randSpeed3*worldSpeed;
        nn->destination.x = nv->destination.x;
        bp->destination.x = bc->destination.x;

        if(Button_IsPressed(atk, mousePos) && whoseTurn.animationBool){
            Player_setAction(player, ATTACK);
        }

        if(Button_IsPressed(def, mousePos) && whoseTurn.animationBool){
            Player_setDefense(player, true);
            playerTookAction = true;
        }

        if(Button_IsPressed(item, mousePos)){
            inventoryShowing = true;
        }

        if(IsKeyPressed(KEY_ESCAPE)){
            inventoryShowing = false;
        }

        if(playerDecision == DEAD || enemyDecision == DEAD){
            switchTurns = false;
        }

        BeginDrawing();
        BeginMode2D(camera);
            ClearBackground(BLACK);
            Image_DrawPro(background);

            if(IsKeyPressed(KEY_ESCAPE)){
                int retural = menuOpen();
                if(retural == 3){
                    CloseWindow();
                }
            }

            if(introduction){
                elapsedN += deltaTime;
                
                float pg = elapsedN / 2.0f;
                float t = pow(pg, 0.5f);

                bc->destination.y = Slerp(startPointC, finalPointC, t);
                bb->destination.y = Slerp(startPointB, finalPointB, t);

                if(pg > 1.0f){
                    Player_MoveTo(enemy, (Vector2){(GetScreenWidth() - 130.0f) - enemyDestRec.width/2, playerDestRec.y + playerDestRec.height - enemyDestRec.height}, 1.0f);
                    Player_MoveTo(player, (Vector2){130.0f - playerDestRec.width/2, 350.0f - playerDestRec.height}, 1.0f);

                    introduction = false;
                    elapsedN = 0;
                }
            }

            Player_Draw(player);
            Player_Draw(enemy);

            healthBarSprite->destination = (Rectangle){enemyDestRec.x + enemyDestRec.width/2 - enemyDestRec.width*2/8, enemyDestRec.y + enemyDestRec.height*1/3, enemyDestRec.width*2/4, 20};
            enemyTookDamage = healthBar(healthBarSprite, healthBarFiller, (Vector2){healthBarSprite->destination.x, healthBarSprite->destination.y}, enemyStats.health, enemyStats.maxHealth, deltaTime);

            healthBarSpriteP->destination = (Rectangle){playerDestRec.x + playerDestRec.width/2 - playerDestRec.width*2/8, playerDestRec.y + playerDestRec.height*1/3, playerDestRec.width*2/4, 20};
            playerTookDamage = healthBarP(healthBarSpriteP, healthBarFillerP, (Vector2){healthBarSpriteP->destination.x, healthBarSpriteP->destination.y}, playerStats.health, playerStats.maxHealth, deltaTime);

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
            LinkButtonColors(itemsQntWep, item, statsOpacity, BLACK);
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

            if(whoseTurn.animationBegin) cameraZoomIn = !cameraZoomIn;

            if(switchTurns){
                whoseTurn = changeTurn(deltaTime);

                if(whoseTurn.animating) worldSpeed = 0.5f;
                else worldSpeed = 1.0f;

                if(whoseTurn.animationEnd){
                    switchTurns = false;
                    (whoseTurn.animationBool) ? printf("\n--------- Switched to %s's turn ---------", Player_getName(player)) :printf("\n--------- Switched to %s's turn ---------", Player_getName(enemy));;
                    enemyHasAttacked = false;
                    playerTookAction = false;

                    if(whoseTurn.animationBool){
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
                
                if(inventoryAnimation.animationEnd) inventoryShowing = false;
            }

            Rectangle posHP = {healthBarSpriteP->destination.x + healthBarSpriteP->destination.width*1.1f, healthBarSpriteP->destination.y - 3, 0, healthBarSpriteP->destination.height};
            Rectangle posHE = {healthBarSprite->destination.x + healthBarSprite->destination.width*1.1f, healthBarSprite->destination.y - 3, 0, healthBarSprite->destination.height};

            LogHealthP(playerStats.health, textP, posHP, deltaTime);
            LogHealthE(enemyStats.health, textE, posHE, deltaTime);

            SpriteSheet_Draw(confetti);

            Image_DrawPro(inventoryBackground);

            // PARAMETRIZAÇÃO DO INVENTARIO
            percorrerLista(Player_getListaInventario(player), imprimirInventario);
            //

            Image_Draw(cursor);
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

    ImageObject* parede = Image_Init("sprites/white.png");
    ImageObject* borda = Image_Init("sprites/white.png");
    ImageObject* chaoTileSet = Image_Init("sprites/tileGrama.png");

    ImageObject* crosshair = Image_Init("sprites/white.png");

    ImageObject* paredesTileSet = Image_Init("sprites/paredes.png");

    paredesTileSet->source = (Rectangle){0, 0, 90, 90};
    paredesTileSet->destination = (Rectangle){0, 0, squaresize/2, squaresize/2};

    Image_Resize(parede, squaresize, squaresize);
    Image_Resize(borda, squaresize, squaresize);
    Image_Resize(chaoTileSet, squaresize * 1.2f, squaresize * 1.2f);
    Image_Resize(crosshair, squaresize, squaresize);

    crosshair->color = (Color){255, 255, 255, 30};
    parede->color = (Color){0, 0, 30, 150};
    borda->color = (Color){80, 0, 0, 255};
    chaoTileSet->color = (Color){30, 90, 30, 255};

    int** mapa = NULL;
    criarMapa(&mapa, TAM);

    srand(time(NULL));

    Player_SetMoveSet(player, (MoveSet){KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT});
    Player_SetStepSize(player, squaresize);

    Player_SetStepSize(enemy, squaresize);
    Player_ChangeCharacter(enemy);

    ImageObject* grama = Image_Init("sprites/grass.png");
    grama->color = (Color){200,200,200,255};

    //////////////////////

    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = cameraInitZoom;

    //////////////////////

    bool placedEnemy = false;

    // INICIALIZA O MAPA
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            if(mapa[i][j] == 1) Player_MoveTo(player, (Vector2){i*squaresize, j*squaresize}, 0.0f);

            CreateGround(mapa, i, j, squaresize, chao, chaoTileSet);

            // WALLS
            HandleTile(mapa, i, j, squaresize, paredes, paredesTileSet);

            if(mapa[i][j] != 0) continue;

            // GENERATE GRASS ONLY ON '0' SQUARES
            generateGrass(effects, grama, (Vector2){i, j}, squaresize);

            int chance2 = rand() % 100;

            if(!placedEnemy && chance2 <= 5){
                placedEnemy = true;
                Player_MoveTo(enemy, (Vector2){(int)i*squaresize, (int)j*squaresize}, 0.0f);
            }
        }
    }

    bool triggerCutscene = false;

    CutsceneFunctions battleTransition = {quadraticFunction, battleTransitionF, battleTransitionR};

    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousepos = GetScreenToWorld2D(GetMousePosition(), camera);

        Player_Update(player, deltaTime);
        Player_Update(enemy, deltaTime);

        Rectangle playerDestRec = Player_getDestRec(player);

        camera.target = (Vector2){playerDestRec.x + playerDestRec.width/2, playerDestRec.y + playerDestRec.height/2};

        moveRandom(enemy, 1.5f);

        if(IsKeyPressed(KEY_ESCAPE)){
            int returnal = menuOpen();
            if(returnal == 3){
                CloseWindow();
            }
        }
        if(IsKeyPressed(KEY_P)){
            triggerCutscene = true;
        }

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
            
            Player_Draw(player);
            Player_Draw(enemy);

            if(cutsceneListener(triggerCutscene, 0.5f, deltaTime, battleTransition)){
                triggerCutscene = false;
                EndMode2D();
                EndDrawing();
                return BATTLE;
            }

            imprimirListaRel(effects, player, imprimirImageObjectProFW);

            crosshair->x = posX * squaresize;
            crosshair->y = posY * squaresize;
            Image_Draw(crosshair);

            EndMode2D();

        EndDrawing();
    }

    return 0;
}

int telaInicial(){
    return 0;
}

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Teste");
    SetTargetFPS(60);

    _chdir(GetApplicationDirectory());
    SetExitKey(KEY_A);

    GAMESTATE gamestate = FREE;

    float screensize = (SCREEN_WIDTH + SCREEN_HEIGHT) / 2;
    float squaresize = screensize/TAM;

    Rectangle source = {0.0f, 120.0f, 160.0f, 200.0f};
    Rectangle dest = {0, 0, squaresize, squaresize};

    Player* player = Player_Init(source, dest, "sprites/Player_spritesheet.png");
    Player* enemy = Player_Init(source, dest, "sprites/Player_spritesheet.png");

    Resources resources = {player, enemy, squaresize};

    while(!WindowShouldClose()){
        switch (gamestate){
            case MAINSCREEN:
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
