#include "TileSet.h"

void CreateGround(int** mapa, int i, int j, int squaresize, Lista* chao, ImageObject* tileSet){
    ImageObject* c = Image_Init(NULL);
    Image_Copy(tileSet, c, true);
    
    if(mapa[i][j] == 2){
        c->color = (Color){15, 80, 15, 255};
    }
    else{
        c->color = (Color){30, 90, 30, 255};
    }

    c->x = squaresize*j;
    c->y = squaresize*i;
    
    inserirFim(chao, c);
}

void HandleTile(int** mapa, int i, int j, int squaresize, Lista* paredes, ImageObject* tileSet){
    if (mapa[i][j] != 2) return;

    bool up = (i > 0) && mapa[i-1][j] == 2;
    bool down = (i < TAM-1) && mapa[i+1][j] == 2;
    bool left = (j > 0) && mapa[i][j-1] == 2;
    bool right = (j < TAM-1) && mapa[i][j+1] == 2;

    bool topleft = (i > 0 && j > 0) && mapa[i-1][j-1] == 2;
    bool topright = (i > 0 && j < TAM-1) && mapa[i-1][j+1] == 2;
    bool bottomleft = (i < TAM-1 && j > 0) && mapa[i+1][j-1] == 2;
    bool bottomright = (i < TAM-1 && j < TAM-1) && mapa[i+1][j+1] == 2;

    ImageObject* tl = Image_Init(NULL);
    ImageObject* tr = Image_Init(NULL);
    ImageObject* bl = Image_Init(NULL);
    ImageObject* br = Image_Init(NULL);

    Image_Copy(tileSet, tl, true);
    Image_Copy(tileSet, tr, true);
    Image_Copy(tileSet, bl, true);
    Image_Copy(tileSet, br, true);

    tl->destination.x = j * squaresize;
    tl->destination.y = i * squaresize;
    tr->destination.x = j * squaresize + squaresize/2;
    tr->destination.y = i * squaresize;
    bl->destination.x = j * squaresize;
    bl->destination.y = i * squaresize + squaresize/2;
    br->destination.x = j * squaresize + squaresize/2;
    br->destination.y = i * squaresize + squaresize/2;

    float deltaX = 90;
    float deltaY = 90;

    if (left){
        tl->source.x = deltaX * 2;
        bl->source.x = 0;

        if (up){
            tl->source.y = deltaY;
            tl->source.x = 0;

            if (topleft){
                tl->source.x = deltaX * 3;
            }
        }
        if (down){
            bl->source.x = deltaX * 3;

            if (bottomleft){
                bl->source.y = deltaY;
            }
        }
    }
    else{
        tl->source.x = deltaX;
        bl->source.x = deltaX;

        if (!up){
            tl->source.y = deltaY;
        }
        if (!down){
            bl->source.x = deltaX * 2;
            bl->source.y = deltaY;
        }
    }
    if (right){
        tr->source.x = deltaX * 2;

        if (up){
            tr->source.x = 0;
            tr->source.y = deltaY;

            if (topright){
                tr->source.x = deltaX * 3;
            }
        }
        if (down){
            br->source.x = deltaX * 3;

            if (bottomright){
                br->source.y = deltaY;
            }
        }

        Image_FlipPro(tr);
        Image_FlipPro(br);
    }
    else{
        tr->source.x = deltaX * 1;
        br->source.x = deltaX * 1;

        if (!up){
            tr->source.x = deltaX;
            tr->source.y = deltaY;
        }
        if (!down){
            br->source.x = deltaX * 2;
            br->source.y = deltaY;
        }

        Image_FlipPro(tr);
        Image_FlipPro(br);
    }

    inserirFim(paredes, tl);
    inserirFim(paredes, tr);
    inserirFim(paredes, bl);
    inserirFim(paredes, br);
}

void UpdateTile(int** mapa, int posX, int posY, int squaresize, Lista* paredes, ImageObject* tileSet){
    HandleTile(mapa, posX, posY, squaresize, paredes, tileSet);
}

void UpdateNeighborTiles(int** mapa, int posY, int posX, int squaresize, Lista* paredes, ImageObject* tileSet){
    UpdateTile(mapa, posX, posY-1, squaresize, paredes, tileSet);
    UpdateTile(mapa, posX-1, posY, squaresize, paredes, tileSet);
    UpdateTile(mapa, posX-1, posY-1, squaresize, paredes, tileSet);

    UpdateTile(mapa, posX-1, posY+1, squaresize, paredes, tileSet);
    UpdateTile(mapa, posX+1, posY-1, squaresize, paredes, tileSet);

    UpdateTile(mapa, posX, posY+1, squaresize, paredes, tileSet);
    UpdateTile(mapa, posX+1, posY, squaresize, paredes, tileSet);
    UpdateTile(mapa, posX+1, posY+1, squaresize, paredes, tileSet);
}