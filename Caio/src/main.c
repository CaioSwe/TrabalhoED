#pragma region "Structs"

#include "Button.h"
#include "ImageObject.h"
#include "Gif.h"
#include "TextObject.h"
#include "Input.h"
#include "Player.h"
#include "TileSet.h"
#include "Game.h"

#include "Lista.h"
#include "Arvore.h"

#include "Mapa.h"

#pragma endregion "Structs"

Player* player;
Player* enemy;

Lista* allItems;

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

void imprimirImageObjectProBH(const void* item) {
    const ImageObject* image = (const ImageObject*)item;

    bool isInsidePlayerX = (image->destination.x < (player->destination.x + player->destination.width) && image->destination.x > player->destination.x);
    bool isInsidePlayerY = (image->destination.y < (player->destination.y + player->destination.height) && image->destination.y > (player->destination.y));

    bool isInsidePlayer = isInsidePlayerX && isInsidePlayerY;

    if(isInsidePlayer && (image->destination.y + image->destination.height) < (player->destination.y + player->destination.height)){
        Image_DrawPro((ImageObject*)image);
    }
    else if(!isInsidePlayer){
        imprimirImageObjectPro(image);
    }
}

void imprimirImageObjectProFW(const void* item) {
    const ImageObject* image = (const ImageObject*)item;

    bool isInsidePlayerX = (image->destination.x < (player->destination.x + player->destination.width) && image->destination.x > player->destination.x);
    bool isInsidePlayerY = (image->destination.y < (player->destination.y + player->destination.height) && image->destination.y > (player->destination.y));

    bool isInsidePlayer = isInsidePlayerX && isInsidePlayerY;

    if(isInsidePlayer && (image->destination.y + image->destination.height) >= (player->destination.y + player->destination.height)){
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
    Player_TakeDamage((Player*)item, 10.0f);
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

void placeEnemy(Vector2 coords, float squareSize){
    static bool placedEnemy = false;
    int chance2 = rand() % 100;

    if(!placedEnemy && chance2 <= 5){
        placedEnemy = true;
        Player_MoveTo(enemy, (Vector2){(int)coords.x*squareSize, (int)coords.y*squareSize}, 0.0f);
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

typedef struct Turn{
    bool animationEnd;
    bool animationBegin;
    bool animationBool;

    bool animating;
} Turn;

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
    static bool animBegan = false;

    static Rectangle yourTurn = (Rectangle){0, SCREEN_HEIGHT/2, SCREEN_WIDTH, 0};
    
    TextObject* yourTurnText = (playersTurn) ? Text_Init("Your Turn!") : Text_Init("Enemys Turn!");
    yourTurnText->fontsize = yourTurn.height*2/3;
    
    Text_Pos(yourTurnText, (Vector2){GetScreenWidth()/2 - MeasureText(yourTurnText->text, yourTurnText->fontsize)/2, yourTurn.y + yourTurn.height/2 - yourTurnText->fontsize/2});

    animBegan = (elapsed == 0);

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
    float t2 = pow(sin(3*3.14*pg), 2);

    healthBarColor->r = Slerp(255, 0, t2);
    healthBarColor->b = Slerp(255, 0, t2);

    if(pg > 1.0f){
        elapsed = 0;
        healthBarColor->b = 255;
        healthBarColor->r = 255;
        return true;
    }
    return false;
}

void healthBar(ImageObject* healthBarSprite, ImageObject* healthBarFiller, Vector2 initialPos, float health, float maxHealth, float deltaTime){
    static float relativeHealth = 10; // MUDAR
    static float elapsed = 0;
    static bool hurt = false;
    static bool healed = false;
    static Color healthBarColor = WHITE;
    static Color healthBarBorderColor = WHITE;

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

int fightScreen(void* enemys){
    Player* enemyLocal = (Player*)enemys;

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

    bool changePlayer = true;

    if(changePlayer){
        Player_SetSpriteSheet(player, "sprites/PlayerAnim/noBKG_KnightIdle_strip.png");
        player->display = (Vector2){64.0f, 0.0};
        player->deltaX = 64.0f;
        player->anim->frames = (FramesAnimation){true, 0.0f, 0.0f, 1, 0, 15, 0, 11};

        Player_SetSourceRec(player, (Rectangle){0.0f, 0.0f, 64.0f, 64.0f});
        Player_SetDestRec(player, (Rectangle){-240, GetScreenHeight()/2 - 240/2, 240, 240});

        Player_MoveTo(player, (Vector2){130.0f - player->destination.width/2, 300.0f - player->destination.height/2}, 1.2f);
    }

    bool changeEnemy = true;

    if(changeEnemy){
        Player_SetSpriteSheet(enemyLocal, "sprites/skeletonAnim/skeletonIdle.png");
        enemyLocal->characterChoice = 0.0f;
        enemyLocal->display = (Vector2){0.0f, 0.0f};
        enemyLocal->deltaX = 65.25f;
        enemyLocal->anim->frames = (FramesAnimation){true, 0.0f, 0.0f, 1, 0, 4, 0, 7};

        Player_SetSourceRec(enemyLocal, (Rectangle){0.0f, 0.0f, 65.25f, 64.0f});
        Player_SetDestRec(enemyLocal, (Rectangle){0, 0, (GetScreenWidth()/3) * 0.85f, (GetScreenWidth()/3) * 0.85f});
        Player_MoveTo(enemyLocal, (Vector2){GetScreenWidth(), 306.0f - enemyLocal->destination.height/2}, 0.0f);
    }

    ChangePositionFunction(enemyLocal->anim->position, easedFunction);

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

    Button* health = Button_Init(TextFormat("%.0f/%.0f", player->stats.health, player->stats.maxHealth));
    Button* atkStat = Button_Init(TextFormat("%.1f", player->stats.attack));
    Button* defStat = Button_Init(TextFormat("%.1f%%", player->stats.defense * 100));
    Button* itemsQntUtils = Button_Init(TextFormat("%d", listaTamanho(player->inventario.utils)));
    Button* itemsQntWep = Button_Init(TextFormat("%d", listaTamanho(player->inventario.weapons)));
    Button* runStat = Button_Init(TextFormat("%.1f%%", player->stats.evasionRate * 100));

    Button* healthEnemy = Button_Init(TextFormat("%.0f/%.0f", enemyLocal->stats.health, enemyLocal->stats.maxHealth));
    Button* atkStatEnemy = Button_Init(TextFormat("%.1f", enemyLocal->stats.attack));
    Button* defStatEnemy = Button_Init(TextFormat("%.1f%%", enemyLocal->stats.defense));

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

    Arvore* enemyChoice = criaFolha(isLife50Void, enemyLocal);
    inserirEsqArvore(enemyChoice, isLife30Void, player);
    inserirDirArvore(enemyChoice, Player_PrintVoid, enemyLocal);
    inserirEsqArvore(enemyChoice->dir, attackVoid, player);

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

    float newHeight = GetScreenHeight()*0.8f;
    float newWidth = (inventoryBackground->image.width * newHeight)/inventoryBackground->image.height;

    Rectangle backgorundDest = {GetScreenWidth()/2 - newWidth/2, GetScreenHeight(), newWidth, newHeight};

    inventoryBackground->destination = backgorundDest;

    bool inventoryShowing = false;
    Turn inventoryAnimation = {false, false, false, false};

    ImageObject* healthBarSprite = Image_Init("sprites/healthBar.png");
    ImageObject* healthBarFiller = Image_Init("sprites/healthBarBar.png");

    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousePos = GetMousePosition();
        cursor->x = mousePos.x;
        cursor->y = mousePos.y;

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            printf("\nx = %f, y = %f", mousePos.x, mousePos.y);
        }

        if(Wait(0.3f)){
            randSpeed1 = (randSpeed1 <= minSpeed) ? minSpeed : randSpeed1 - 1;
            randSpeed2 = (randSpeed2 <= minSpeed) ? minSpeed : randSpeed2 - 1;
        }

        player->anim->frames.animating = true;

        Player_UpdatePosition(player, deltaTime);
        Player_UpdateSize(player, deltaTime);
        Player_UpdateSprite(player, false, false);

        Player_UpdatePosition(enemyLocal, deltaTime);
        Player_UpdateSize(enemyLocal, deltaTime);
        Player_UpdateSprite(enemyLocal, false, false);

        bc->destination.x = (bc->destination.x - randSpeed1 <= -bc->destination.width/2.0f) ? 0 : bc->destination.x - randSpeed1;
        bb->destination.x = (bb->destination.x - randSpeed2 <= -bb->destination.width/2.0f) ? 0 : bb->destination.x - randSpeed2;
        nv->destination.x = (nv->destination.x - randSpeed3 <= -nv->destination.width/2.0f) ? 0 : nv->destination.x - randSpeed3;
        nn->destination.x = nv->destination.x;
        bp->destination.x = bc->destination.x;

        if(Button_IsPressed(atk, mousePos) && whoseTurn.animationBool){
            Player_TakeDamage(enemy, player->stats.attack);
            think = true;
        }

        if(Button_IsPressed(item, mousePos)){
            inventoryShowing = true;
        }

        if(IsKeyPressed(KEY_ESCAPE)){
            inventoryShowing = false;
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

            if(!whoseTurn.animationBool && whoseTurn.animationEnd){
                think = true;
            }

            if(Wait2(((double)(0.5f))) && think){
                if(!whoseTurn.animationBool) percorrerArvore(enemyChoice);
                switchTurns = true;
                think = false;
            }

            if(introduction){
                elapsedN += deltaTime;
                
                float pg = elapsedN / 2.0f;
                float t = pow(pg, 0.5f);

                bc->destination.y = Slerp(startPointC, finalPointC, t);
                bb->destination.y = Slerp(startPointB, finalPointB, t);

                if(pg > 1.0f){
                    Player_MoveTo(enemyLocal, (Vector2){400.0f - enemyLocal->destination.width/2, 306.0f - enemyLocal->destination.height/2}, 2.5f);

                    introduction = false;
                    elapsedN = 0;
                }
            }

            Player_Draw(player);

            Player_Draw(enemyLocal);

            healthBarSprite->destination = (Rectangle){enemyLocal->destination.x + enemyLocal->destination.width/2 - enemyLocal->destination.width*3/8, enemyLocal->destination.y - 20, enemyLocal->destination.width*3/4, 20};
            healthBar(healthBarSprite, healthBarFiller, (Vector2){healthBarSprite->destination.x, healthBarSprite->destination.y}, enemyLocal->stats.health, enemyLocal->stats.maxHealth, deltaTime);

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

            Button_SetText(health, TextFormat("%.0f/%.0f", player->stats.health, player->stats.maxHealth));
            Button_SetText(healthEnemy, TextFormat("%.0f/%.0f", enemyLocal->stats.health, enemyLocal->stats.maxHealth));

            percorrerLista(statsButtons, Button_DrawIconVoid);

            Button_DrawIcon(atk);
            Button_DrawIcon(def);
            Button_DrawIcon(item);
            Button_DrawIcon(run);
            
            Button_Draw(showStats);

            if(whoseTurn.animationBegin) cameraZoomIn = !cameraZoomIn;

            if(switchTurns){
                whoseTurn = changeTurn(deltaTime);

                if(whoseTurn.animationEnd){
                    switchTurns = false;
                    printf("\nSwitched to ");
                    (whoseTurn.animationBool) ? printf("players turn.") : printf("enemys turn.");
                }
            }

            if(cameraZoomIn){            
                bool w = cameraAnimation(&camera, (Vector2){GetScreenWidth()/2, GetScreenHeight()/2}, (Vector2){enemyLocal->destination.x + enemyLocal->destination.width/2, enemyLocal->destination.y + enemyLocal->destination.height/2}, deltaTime);
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

            Image_DrawPro(inventoryBackground);

            Image_Draw(cursor);
        EndDrawing();
    }
    return 0;
}

int telaJogo(){
    Lista* effects = criaLista();
    Lista* paredes = criaLista();
    Lista* chao = criaLista();

    ImageObject* parede = Image_Init("sprites/white.png");
    ImageObject* borda = Image_Init("sprites/white.png");
    ImageObject* chaoTileSet = Image_Init("sprites/tileGrama.png");

    ImageObject* crosshair = Image_Init("sprites/white.png");

    ImageObject* paredesTileSet = Image_Init("sprites/paredes.png");
    
    float screensize = (SCREEN_WIDTH + SCREEN_HEIGHT) / 2;
    float squaresize = screensize/TAM;

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

    Rectangle source = {0.0f, 120.0f, 160.0f, 200.0f};
    Rectangle dest = {0, 0, squaresize, squaresize};

    player = Player_Init(source, dest, "sprites/Player_spritesheet.png");
    
    enemy = Player_Init(source, dest, "sprites/Player_spritesheet.png");

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
            placeEnemy((Vector2){i, j}, squaresize);
        }
    }

    // NAO SEI SE DEIXO ISSO
    ImageObject* selectedTileSet = paredesTileSet;
    Lista* selectedList = paredes;

    bool triggerCutscene = false;

    CutsceneFunctions battleTransition = {quadraticFunction, battleTransitionF, battleTransitionR};

    while(!WindowShouldClose()){
        float deltaTime = GetFrameTime();
        Vector2 mousepos = GetScreenToWorld2D(GetMousePosition(), camera);

        Player_Update(player, deltaTime);
        Player_Update(enemy, deltaTime);

        camera.target = (Vector2){player->destination.x + player->destination.width/2, player->destination.y + player->destination.height/2};

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
            imprimirLista(effects, imprimirImageObjectProBH);
            
            Player_Draw(player);
            Player_Draw(enemy);

            imprimirLista(effects, imprimirImageObjectProFW);

            crosshair->x = posX * squaresize;
            crosshair->y = posY * squaresize;
            Image_Draw(crosshair);

            EndMode2D();

            if(cutsceneListener(triggerCutscene, 0.5f, deltaTime, battleTransition)){
                triggerCutscene = false;
                EndDrawing();
                changeScreenItem(fightScreen, enemy);
            }

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

    changeScreen(telaJogo);

    return 0;
}
