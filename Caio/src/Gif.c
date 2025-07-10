#include "Gif.h"

Gif* Gif_Init(const char* path, const char* ext, int numFrames){
    Gif* gif = (Gif*)malloc(sizeof(Gif));
    
    gif->numFrames = numFrames;
    for (int i = 0; i < numFrames; i++){
        char filename[64];
        sprintf(filename, "%sframe%d%s", path, i, ext);
        gif->image[i] = LoadTexture(filename);
    }
    gif->color = WHITE;
    gif->x = gif->y = 0;
    gif->currentFrame = 0;
    gif->frameTime = 0.2f;
    gif->timer = 0;
    gif->rotation = 0;

    return gif;
}

void Gif_Scale(Gif* gif, float scaling){
    float yscale = (float)(GetScreenHeight() * scaling) / gif->image[0].height;
    float xscale = (float)(GetScreenWidth() * scaling) / gif->image[0].width;
    gif->scale = fmax(xscale, yscale);
}

void Gif_Update(Gif* gif){
    gif->timer += GetFrameTime();
    if (gif->timer >= gif->frameTime){
        gif->timer = 0;
        gif->currentFrame = (gif->currentFrame + 1) % gif->numFrames;
    }
}

void Gif_Draw(Gif* gif){
    DrawTextureEx(gif->image[gif->currentFrame], (Vector2){gif->x, gif->y}, gif->rotation, gif->scale, gif->color);
}