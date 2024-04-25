#include <iostream>
#include <SDL.h>
#include <math.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "DefinedValues.h"
#include "Engine.h"
#include "Text.h"
#include "Sound.h"



using namespace std;




int main(int argc, char *argv[])
{
    GameEngine engine;
    engine.init();
    initSound();
    Mix_PlayMusic(MusicAudio,-1);
    while (true)
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (GameState)
        {
            case S_Menu:
                {
                    while (GameState==S_Menu)
                    {
                        SDL_PollEvent(&event);
                        engine.MenuLoad(event);
                    }
                    break;
                }
            case S_Game:
                {
                    while (GameState==S_Game)
                    {
                            engine.run();
                    }
                    break;
                }
            case S_Shop:
                {
                    while (GameState==S_Shop)
                    {
                        SDL_PollEvent(&event);
                        engine.shop(event);
                    }
                    break;
                }
            case S_Setting:
                {
                    while (GameState==S_Setting)
                    {
                        SDL_PollEvent(&event);
                        engine.setting(event);
                    }
                    break;
                }
            case S_Paused:
                {
                    while (GameState==S_Paused)
                    {
                    engine.paused();
                    }
                    break;
                }
            case S_DeathMenu:
                {
                    engine.DeathMenu();
                    break;
                }
        }
    }
    engine.close();

    return 0;
}
