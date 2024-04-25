#ifndef _Text__H_
#define _Text__H_

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "DefinedValues.h"
#include "Graphics.h"

using namespace std;


    void initText()
    {
        TTF_Init();
        font = TTF_OpenFont("assets/Minecraft.ttf",45);
    }
    void write(char* s , int x,int y)
    {
        SDL_Rect dsrect;
        dsrect.x=x;
        dsrect.y=y;
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font,s,textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        dsrect.w = textSurface->w;
        dsrect.h = textSurface->h;
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTexture,NULL,&dsrect);
    }
    char* convertItoC(int x)
    {
        char buffer[10];
        sprintf(buffer, "%d", x);
        char *result = buffer;
        return result;
    }



#endif // _Text__H_
