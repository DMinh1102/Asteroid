#ifndef _Graphics__H
#define _Graphics__H

#include "DefinedValues.h"
#include <iostream>

using namespace std;

void loadMedia();

void initSDL()
            {
                window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |SDL_RENDERER_PRESENTVSYNC);
                loadMedia();
                ifstream Score("Score.txt");
                Score>>totalScore;
                Score.close();
            }
void renderTexture(SDL_Texture *texture, float x, float y,int w,int h ,SDL_Renderer* renderer, double angle)
            {
                SDL_Rect dest;
                dest.x = x;
                dest.y = y;
                dest.w = w;
                dest.h = h;
                SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE );
            }
void renderTextureAt(SDL_Texture *texture,SDL_Rect srect, SDL_Rect drect)
            {
                SDL_RenderCopy(renderer,texture, &srect, &drect);

            }
SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer)
            {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

                SDL_Texture *texture = IMG_LoadTexture(renderer, filename);

                return texture;
            }
void loadMedia()
            {
                IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
                PlayerTexture=loadTexture("assets/Player.png",renderer);
                ProjectileTexture=loadTexture("assets/projectile.png",renderer);
                MeteorTexture=loadTexture("assets/meteor.png",renderer);
                PlayButton=loadTexture("assets/PlayButton.png",renderer);
                SettingButton=loadTexture("assets/SettingButton.png",renderer);
                ShopButton=loadTexture("assets/ShopButton.png",renderer);
                ExitButton=loadTexture("assets/ExitButton.png",renderer);
                BG=loadTexture("assets/BG.png",renderer);
                Damage=loadTexture("assets/Damage.png",renderer);
                Speed=loadTexture("assets/Speed.png",renderer);
                Health=loadTexture("assets/Health.png",renderer);
                CloseButton=loadTexture("assets/CloseButton.png",renderer);
                Frame=loadTexture("assets/Frame.png",renderer);
                IGBG=loadTexture("assets/IGBG.png",renderer);
                ShopList=loadTexture("assets/ShopList.png",renderer);
                FireRate=loadTexture("assets/FireRate.png",renderer);
                HomeButton=loadTexture("assets/HomeButton.png",renderer);
                DeathBG=loadTexture("assets/DeathBG.png",renderer);
                HomeButton=loadTexture("assets/HomeButton.png",renderer);
                ReplayButton=loadTexture("assets/ReplayButton.png",renderer);
                Selected=loadTexture("assets/Selected.png",renderer);
                UpgradeGray=loadTexture("assets/UpgradeGray.png",renderer);
                UpgradeWhite=loadTexture("assets/UpgradeWhite.png",renderer);
                Name=loadTexture("assets/Name.png",renderer);
                GameOver=loadTexture("assets/GameOver.png",renderer);
                MusicButtonOff=loadTexture("assets/MusicButtonOff.png",renderer);
                MusicButtonOn=loadTexture("assets/MusicButtonOn.png",renderer);
                SoundButton=loadTexture("assets/SoundButton.png",renderer);
                PauseButton=loadTexture("assets/PauseButton.png",renderer);

            }







#endif // _SetupGraphics__H
