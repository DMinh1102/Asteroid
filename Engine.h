#ifndef _Engine__H
#define _Engine__H

#include <bits/stdc++.h>
#include "DefinedValues.h"
#include "Graphics.h"
#include "Text.h"
#include "Sound.h"
using namespace std;


struct Object
{
    float Angle;
    SDL_Rect rect;
    int ObjectDamage;
    bool active;
    int health;
    int score=0;

};


Object Player;
Object Projectiles[MaxProjectiles];
Object Meteor[MaxMeteors];

void Hit(Object &a, Object &b);
void Destroy(Object &x);
bool CollisionDetect(Object &a, Object &b);
void HitFire();
void MeteorBounce();

struct GameEngine
{
        void init()
            {
                setup();
                initSDL();
                initText();
            }
        void setup()
            {
                readStats();
                Player.ObjectDamage=100;
                Player_SPEED = 5 + SpeedLevel*2;
                Player.health = 3 + HealthLevel;
                COOLDOWN_DURATION=500-FireRateLevel*100;
                Player.rect.x = (SCREEN_WIDTH - PLAYER_WIDTH) / 2;
                Player.rect.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
                Player.active = true;
                Player.rect.h = PLAYER_HEIGHT;
                Player.rect.w = PLAYER_WIDTH;
                for (int i = 0; i < MaxMeteors; ++i)
                {
                    Meteor[i].rect.w=METEOR_WIDTH;
                    Meteor[i].rect.h=METEOR_HEIGHT;
                    Meteor[i].rect.x=SCREEN_WIDTH*2;
                    Meteor[i].rect.y=SCREEN_HEIGHT*2;
                    Meteor[i].active=false;
                    Meteor[i].ObjectDamage=1;
                }
                for (int i = 0; i < MaxProjectiles; ++i)
                {
                    Projectiles[i].ObjectDamage=1+DamageLevel;
                    Projectiles[i].rect.h = PROJECTILE_HEIGHT;
                    Projectiles[i].rect.w = PROJECTILE_WIDTH;
                    Projectiles[i].rect.x = SCREEN_WIDTH*2;
                    Projectiles[i].rect.y = SCREEN_HEIGHT*2;
                    Projectiles[i].active = false;
                }
            }
        void readStats()
            {
                ifstream Stats("SavedGame.txt");
                Stats>>DamageLevel>>SpeedLevel>>HealthLevel>>FireRateLevel;
            }
        void updateStats()
            {
                ifstream Stats("SavedGame.txt");
                ofstream Temp("Temp.txt");
                Temp<<DamageLevel<<' '<<SpeedLevel<<' '<<HealthLevel<<' '<<FireRateLevel;
                Stats.close();
                Temp.close();
                remove("SavedGame.txt");
                rename("Temp.txt","SavedGame.txt");
            }
        void HandleEvents()
            {
            int x,y;
            SDL_GetMouseState(&x, &y);
            Player.Angle=atan2(Player.rect.y - y, Player.rect.x - x)*180/M_PI;
            SDL_TimerID cooldownTimerID = 0;

            SDL_Event event;

            SDL_PollEvent(&event);

            if (event.type==SDL_QUIT) exit(0);
            const Uint8* state = SDL_GetKeyboardState(nullptr);
            if (state[SDL_SCANCODE_W] and Player.rect.y>0) {
                Player.rect.y -= Player_SPEED;
            }
            if (state[SDL_SCANCODE_S] and Player.rect.y<=SCREEN_HEIGHT-PLAYER_HEIGHT-10) {
                Player.rect.y += Player_SPEED;
            }
            if (state[SDL_SCANCODE_A] and Player.rect.x>0) {
                Player.rect.x -= Player_SPEED;
            }
            if (state[SDL_SCANCODE_D] and Player.rect.x<=SCREEN_WIDTH-PLAYER_WIDTH-10) {
                Player.rect.x += Player_SPEED;
            }
            if (state[SDL_SCANCODE_SPACE] and firecooldown==false) {
                for (int i = 0; i < MaxProjectiles; ++i) {
                    if (Projectiles[i].active == false) {
                playSound(fireSound);
                Projectiles[i].health = 1;
                Projectiles[i].rect.x = Player.rect.x+PLAYER_WIDTH/2-PROJECTILE_WIDTH/2;
                Projectiles[i].rect.y = Player.rect.y+PLAYER_HEIGHT/2-PROJECTILE_HEIGHT/2;
                if (Projectiles[i].active==false) Projectiles[i].Angle=Player.Angle;
                Projectiles[i].active=true;
                firecooldown = true;
                if (cooldownTimerID == 0) cooldownTimerID = SDL_AddTimer(COOLDOWN_DURATION, cooldownTimerCallbackProjectile, NULL);
                break;
            }
                }
            }
            if (state[SDL_SCANCODE_ESCAPE]) GameState=S_Paused;
            HitFire();
        }
        void Update()
            {
                updateVolume();
                for (int i = 0; i < MaxProjectiles; ++i)
                        if (Projectiles[i].active)
                        {
                            Projectiles[i].rect.x-=PROJECTILE_SPEED*cos(Projectiles[i].Angle*M_PI/180.0f);
                            Projectiles[i].rect.y-=PROJECTILE_SPEED*sin(Projectiles[i].Angle*M_PI/180.0f);
                        }

                for (int i = 0; i < MaxMeteors; ++i)
                    if (Meteor[i].active)
                        {
                            Meteor[i].rect.x-=(METEOR_SPEED*cos(Meteor[i].Angle*M_PI/180.0f));
                            Meteor[i].rect.y-=(METEOR_SPEED*sin(Meteor[i].Angle*M_PI/180.0f));
                        }
                for (int i = 0; i < MaxProjectiles; ++i)
                    if (Projectiles[i].rect.x>SCREEN_WIDTH or Projectiles[i].rect.x<0 or Projectiles[i].rect.y>SCREEN_HEIGHT or Projectiles[i].rect.y<0)
                        {
                            Destroy(Projectiles[i]);
                        }
                for (int i = 0; i < MaxMeteors; ++i)
                    if (Meteor[i].rect.y>SCREEN_HEIGHT+50 or Meteor[i].rect.x>SCREEN_WIDTH+50)
                        {
                            Destroy(Meteor[i]);
                        }
                for (int i=0; i < MaxMeteors; ++i)
                    {
                        if (CollisionDetect(Meteor[i],Player))
                        {
                            Destroy(Meteor[i]);
                            Player.health--;
                            if (Player.health<=0) Destroy(Player);
                        }
                    }
                CurrentTime = SDL_GetTicks();
                TimePassed = CurrentTime - StartTime;
                if (TimePassed%8000==0 and Player.health>0)
                {
                    METEOR_SPEED=2+TimePassed/8000;
                    COOLDOWN_METEOR=3000-TimePassed/8000*400;
                }
                if (TimePassed%20000==0 and Player.health>0)
                {
                    METEOR_HEALTH=2+TimePassed/20000;
                }
            }
        void Render()
            {
                SDL_RenderClear(renderer);

                renderTexture(IGBG,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,renderer,0);
                renderTexture(PlayerTexture, Player.rect.x,Player.rect.y,Player.rect.w,Player.rect.h,renderer,Player.Angle-90);
                for (int i = 0; i < MaxProjectiles; ++i)
                    {
                        if (Projectiles[i].active == true ) {
                                {
                                    if (Projectiles[i].health!=0) renderTexture(ProjectileTexture, Projectiles[i].rect.x,Projectiles[i].rect.y,Projectiles[i].rect.w,Projectiles[i].rect.h,renderer, Projectiles[i].Angle+90);
                                    else Projectiles[i].active = false;
                                }
                        }
                    }

                for (int i = 0; i < MaxMeteors; ++i)
                    {
                        if (Meteor[i].active== true)
                            {
                                if (Meteor[i].health!=0) renderTexture(MeteorTexture, Meteor[i].rect.x ,Meteor[i].rect.y,Meteor[i].rect.w,Meteor[i].rect.h,renderer,NULL);
                                else Meteor[i].active = false;
                            }
                    }
                write("Health:",900,5);
                write(convertItoC(Player.health),1060,5);
                write("Score: ",0,5);
                write(convertItoC(TimePassed/100),150,5);
                SDL_RenderPresent(renderer);
            }
        void close()
            {
                SDL_DestroyTexture(PlayerTexture);
                SDL_DestroyTexture(MeteorTexture);
                SDL_DestroyTexture(ProjectileTexture);
                SDL_DestroyTexture(PlayButton);
                SDL_DestroyTexture(ExitButton);
                SDL_DestroyTexture(BG);
                SDL_DestroyTexture(Health);
                SDL_DestroyTexture(Speed);
                SDL_DestroyTexture(Damage);
                SDL_DestroyTexture(FireRate);
                SDL_DestroyTexture(CloseButton);
                SDL_DestroyTexture(SpaceBG);
                SDL_DestroyTexture(Frame);
                SDL_DestroyTexture(IGBG);
                SDL_DestroyTexture(ShopList);
                SDL_DestroyTexture(DeathBG);
                SDL_DestroyTexture(ReplayButton);
                SDL_DestroyTexture(HomeButton);
                SDL_DestroyTexture(Selected);
                SDL_DestroyTexture(UpgradeGray);
                SDL_DestroyTexture(UpgradeWhite);
                SDL_DestroyTexture(Name);
                SDL_DestroyTexture(GameOver);
                SDL_DestroyTexture(SoundButton);
                SDL_DestroyTexture(MusicButtonOn);
                SDL_DestroyTexture(MusicButtonOff);
                SDL_DestroyTexture(PauseButton);

                SDL_DestroyRenderer(renderer);

                SDL_DestroyWindow(window);
                Mix_FreeChunk(menuClick);
                Mix_FreeChunk(menuClick2);
                Mix_FreeChunk(fireSound);
                Mix_FreeChunk(deathSound);
                Mix_FreeChunk(upgradeSound);
                Mix_FreeChunk(explosion);
                Mix_FreeMusic(MusicAudio);
                Mix_CloseAudio();
                Mix_Quit();
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
            }
        void SpawnMeteors()
            {
                SDL_TimerID SpawnTimer = 0;

                if (SpawnTimer==0 and spawncooldown==false)
                {
                    int ran=CurrentTime%3;
                    switch(ran)
                    {
                    case 0:
                        {
                            for (int i =0 ; i< MaxMeteors ; ++i)
                                {
                                    if (Meteor[i].active==false)
                                        {
                                            Meteor[i].active=true;
                                            spawncooldown = true;
                                            Meteor[i].health=METEOR_HEALTH;
                                            Meteor[i].rect.x=MeteorSpawnX;
                                            Meteor[i].rect.y=0;
                                            Meteor[i].Angle=270;
                                            MeteorSpawnX+=METEOR_WIDTH+7;
                                        }
                                    if (MeteorSpawnX>SCREEN_WIDTH )
                                        {
                                            SpawnTimer = SDL_AddTimer(COOLDOWN_METEOR, cooldownTimerCallbackMeteor, NULL);
                                            spawncooldown=true;
                                            break;
                                        }
                                }
                            break;
                        }
                    case 1:
                        {
                            for (int i =0 ; i< MaxMeteors ; ++i)
                                {
                                    if (Meteor[i].active==false)
                                        {
                                            Meteor[i].active=true;
                                            spawncooldown = true;
                                            Meteor[i].health=METEOR_HEALTH;
                                            Meteor[i].rect.x=0;
                                            Meteor[i].rect.y=MeteorSpawnY;
                                            Meteor[i].Angle=180;
                                            MeteorSpawnY+=METEOR_WIDTH+7;
                                        }
                                    if (MeteorSpawnY>SCREEN_HEIGHT )
                                        {
                                            SpawnTimer = SDL_AddTimer(COOLDOWN_METEOR, cooldownTimerCallbackMeteor, NULL);
                                            spawncooldown=true;
                                            break;
                                        }
                                }
                            break;
                        }
                    case 2:
                        {
                            for (int i =0 ; i< MaxMeteors ; ++i)
                                {
                                    if (Meteor[i].active==false)
                                        {
                                            Meteor[i].active=true;
                                            spawncooldown = true;
                                            Meteor[i].health=METEOR_HEALTH;
                                            Meteor[i].rect.x=SCREEN_WIDTH;
                                            Meteor[i].rect.y=MeteorSpawnY;
                                            Meteor[i].Angle=0;
                                            MeteorSpawnY+=METEOR_WIDTH+7;
                                        }
                                    if (MeteorSpawnY>SCREEN_HEIGHT )
                                        {
                                            SpawnTimer = SDL_AddTimer(COOLDOWN_METEOR, cooldownTimerCallbackMeteor, NULL);
                                            spawncooldown=true;
                                            break;
                                        }
                                }
                                break;

                        }
                    }
                }

            }
        void HitFire()
            {
                for (int i=0;i<MaxProjectiles; ++i)
                {
                    if (Projectiles[i].active == true)
                        for (int j = 0;j < MaxMeteors; ++j)
                            {
                                if (Meteor[j].active == true)
                                    {
                                        if (CollisionDetect(Projectiles[i],Meteor[j]))
                                            {
                                                Hit(Projectiles[i], Meteor[j]);
                                                if (Meteor[j].health<=0) playSound(explosion);
                                            }
                                    }
                            }
                }

            }
        void Hit(Object &a, Object &b)
            {
                a.health-=b.ObjectDamage;
                b.health-=a.ObjectDamage;
                if (a.health<=0) Destroy(a);
                if (b.health<=0) Destroy(b);
            }
        void Destroy(Object &x)
            {
                x.active=false;
                x.rect.x=SCREEN_WIDTH+200;
                x.rect.y=SCREEN_HEIGHT+200;
            }
        bool CollisionDetect(Object &a, Object &b)
            {
                if ((a.rect.x<= b.rect.x + b.rect.w) and (a.rect.y <=b.rect.y+b.rect.h) and (a.rect.x + a.rect.w >= b.rect.x) and (a.rect.y + a.rect.h >= b.rect.y)) return true;
                else return false;
            }
        void MenuLoad(SDL_Event event)
            {
                SDL_RenderClear(renderer);
                renderTexture(BG,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,renderer,NULL);
                renderTexture(Frame,360,160,480,480,renderer,NULL);
                renderTexture(PlayButton,540,365,120,120,renderer,NULL);
                renderTexture(ShopButton,400,500,120,120,renderer,NULL);
                renderTexture(SettingButton,680,500,120,120,renderer,NULL);
                renderTexture(ExitButton,540,500,120,120,renderer,NULL);
                renderTexture(Name,380,230,450,175,renderer,NULL);
                if (event.type==SDL_QUIT) close();
                if (event.type==SDL_MOUSEBUTTONDOWN)
                    {
                        int x,y;
                        SDL_GetMouseState(&x, &y);
                        SDL_Point MouseClick ={x,y};
                        SDL_Rect PlayButtonRect = {540,365,120,120};
                        SDL_Rect ExitButtonRect = {540,500,120,120};
                        SDL_Rect ShopButtonRect = {400,500,120,120};
                        SDL_Rect SettingButtonRect = {680,500,120,120};
                        if (SDL_PointInRect(&MouseClick,&PlayButtonRect))
                        {
                            playSound(menuClick);
                            startNew();
                            GameState=S_Game;
                        }
                        if (SDL_PointInRect(&MouseClick,&ExitButtonRect))
                        {
                            playSound(menuClick);
                            close();
                        }
                        if (SDL_PointInRect(&MouseClick,&ShopButtonRect))
                        {
                            playSound(menuClick);
                            GameState=S_Shop;
                        }
                        if (SDL_PointInRect(&MouseClick,&SettingButtonRect))
                        {
                            playSound(menuClick);
                            GameState=S_Setting;
                        }
                        updateStats();
                    }
                SDL_RenderPresent(renderer);
            }
        void Death()
            {
                playSound(deathSound);
                Point=TimePassed/100;
                ifstream Score("Score.txt");
                Score>>totalScore>>highScore;
                Score.close();
                totalScore+=Point;
                if (Point>highScore)highScore=Point;
                ReWriteScore(totalScore,highScore);
                cout<<"Your score: "<<Point<<endl;
                cout<<"High score: "<<highScore<<endl;
                GameState=S_DeathMenu;
            }
        void ReWriteScore(int x,int y)
            {
            ifstream Score("Score.txt");
            ofstream Temp("Temp.txt");
            Temp<<x<<endl<<y;
            Score.close();
            Temp.close();
            remove("Score.txt");
            rename("Temp.txt","Score.txt");
        }
        void DeathMenu()
            {
            renderTexture(DeathBG,360,220,480,360,renderer,NULL);
            renderTexture(HomeButton,400,440,96,96,renderer,NULL);
            renderTexture(ReplayButton,550,440,96,96,renderer,NULL);
            renderTexture(ShopButton,700,440,96,96,renderer,NULL);
            renderTexture(GameOver,350,240,500,200,renderer,NULL);
            SDL_RenderPresent(renderer);
            SDL_Rect ShopButtonRect = {700,440,96,96};
            SDL_Rect HomeButtonRect = {400,440,96,96};
            SDL_Rect ReplayButtonRect = {550,440,96,96};
            while (GameState==S_DeathMenu)
            {
                SDL_Event event;
                SDL_PollEvent(&event);
                if (event.type==SDL_MOUSEBUTTONDOWN)
                {
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Point MouseClick ={x,y};
                    if (SDL_PointInRect(&MouseClick,&ReplayButtonRect))
                    {
                        startNew();
                        GameState=S_Game;
                    }
                    if (SDL_PointInRect(&MouseClick,&ShopButtonRect)) GameState=S_Shop;
                    if (SDL_PointInRect(&MouseClick,&HomeButtonRect)) GameState=S_Menu;
                }
            }
        }
        void shop(SDL_Event event)
            {
                int x,y;
                SDL_GetMouseState(&x, &y);
                SDL_Point MouseClick ={x,y};
                SDL_RenderClear(renderer);
                renderTexture(BG,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,renderer,NULL);
                renderTexture(ShopList,275,30,650,735,renderer,NULL);

                UpgradeButton = {330,675,300,100};

                DamageLevelRect = {DamageLevel*32,0,32,32};

                SpeedLevelRect = {SpeedLevel*32,0,32,32};

                HealthLevelRect = {HealthLevel*32,0,32,32};

                FireRateLevelRect = {FireRateLevel*32,0,32,32};

                write("Damage per bullet", 450,80);
                write("Movement speed", 450,235);
                write("Health", 450,382);
                write("Rate of fire", 450,532);
                write((DamageCostT[DamageLevel]),450,140);
                write((SpeedCostT[SpeedLevel]),450,295);
                write((HealthCostT[HealthLevel]),450,442);
                write((FireRateCostT[FireRateLevel]),450,592);
                write(convertItoC(totalScore),700,683);
                renderTextureAt(Damage,DamageLevelRect,DamageLevelLocation);
                renderTextureAt(Health,HealthLevelRect,HealthLevelLocation);
                renderTextureAt(Speed,SpeedLevelRect,SpeedLevelLocation);
                renderTextureAt(FireRate,FireRateLevelRect,FireRateLevelLocation);
                renderTexture(CloseButton,10,10,120,120,renderer,NULL);
                renderTexture(Selected,328,ShopListLocation[ShopSelect],96,96,renderer,NULL);
                switch(ShopSelect)
                {
                case 0:
                    {
                        if (DamageLevel<3 and totalScore>=DamageCost[DamageLevel]) Upgradable =true;
                        else Upgradable=false;
                        if (event.type==SDL_MOUSEBUTTONDOWN and SDL_PointInRect(&MouseClick,&UpgradeButton) and Upgradable)
                        {
                            playSound(upgradeSound);
                            totalScore-=DamageCost[DamageLevel];
                            DamageLevel++;
                        }
                        break;
                    }
                case 1:
                    {
                        if (SpeedLevel<3 and totalScore>=SpeedCost[SpeedLevel]) Upgradable =true;
                        else Upgradable=false;
                        if (event.type==SDL_MOUSEBUTTONDOWN and SDL_PointInRect(&MouseClick,&UpgradeButton) and Upgradable)
                        {
                            playSound(upgradeSound);
                            totalScore-=SpeedCost[SpeedLevel];
                            SpeedLevel++;
                        }
                        break;
                    }
                case 2:
                    {
                        if (HealthLevel<3 and totalScore>=HealthCost[HealthLevel]) Upgradable =true;
                        else Upgradable=false;
                        if (event.type==SDL_MOUSEBUTTONDOWN and SDL_PointInRect(&MouseClick,&UpgradeButton) and Upgradable)
                        {
                            playSound(upgradeSound);
                            totalScore-=HealthCost[HealthLevel];
                            HealthLevel++;
                        }
                        break;
                    }
                case 3:
                    {
                        if (FireRateLevel<3 and totalScore>=FireRateCost[FireRateLevel] ) Upgradable =true;
                        else Upgradable=false;
                        if (event.type==SDL_MOUSEBUTTONDOWN and SDL_PointInRect(&MouseClick,&UpgradeButton) and Upgradable)
                        {
                            playSound(upgradeSound);
                             totalScore-=FireRateCost[FireRateLevel];
                            FireRateLevel++;
                        }
                        break;
                    }
                }
                ReWriteScore(totalScore,highScore);
                if (Upgradable) renderTexture(UpgradeWhite,340,682,250,75,renderer,NULL);
                else renderTexture(UpgradeGray,340,682,250,75,renderer,NULL);


               if (event.type==SDL_MOUSEBUTTONDOWN)
               {
                    SDL_Rect CloseButtonRect = {10,10,105,105};
                    if (SDL_PointInRect(&MouseClick,&CloseButtonRect))
                    {
                        playSound(menuClick);
                        GameState=S_Menu;
                    }
                    if (SDL_PointInRect(&MouseClick,&DamageLevelLocation))
                    {
                        playSound(menuClick2);
                        ShopSelect=0;
                    }
                    if (SDL_PointInRect(&MouseClick,&SpeedLevelLocation))
                    {
                        playSound(menuClick2);
                        ShopSelect=1;
                    }
                    if (SDL_PointInRect(&MouseClick,&HealthLevelLocation))
                    {
                        playSound(menuClick2);
                        ShopSelect=2;
                    }
                    if (SDL_PointInRect(&MouseClick,&FireRateLevelLocation))
                    {
                        playSound(menuClick2);
                        ShopSelect=3;
                    }


               }
               SDL_RenderPresent(renderer);

            }
        void setting(SDL_Event event)
            {
                SDL_RenderClear(renderer);
                MusicButtonRect = {400,500,128,128};
                SoundButtonLocation = {540,490,128,128};
                SoundButtonRect = {Volume*16,0,16,16};
                renderTexture(BG,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,renderer,NULL);
                renderTexture(Frame,360,160,480,480,renderer,NULL);
                if (Music) renderTexture(MusicButtonOn,400,490,128,128,renderer,NULL);
                else renderTexture(MusicButtonOff,400,490,128,128,renderer,NULL);
                renderTexture(CloseButton,10,10,120,120,renderer,NULL);
                renderTextureAt(SoundButton,SoundButtonRect,SoundButtonLocation);
                write("HighScore: ",400,250);
                write(convertItoC(highScore),650,250);
                write("TotalPoint: ",400,350);
                write(convertItoC(totalScore),650,350);

                if (event.type==SDL_MOUSEBUTTONDOWN)
                {
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Point MouseClick ={x,y};
                    SDL_Rect CloseButtonRect = {10,10,105,105};
                    if (SDL_PointInRect(&MouseClick,&CloseButtonRect))
                    {
                        playSound(menuClick);
                        GameState=S_Menu;
                    }
                    if (SDL_PointInRect(&MouseClick,&MusicButtonRect))
                    {
                        playSound(menuClick);
                        if (Music) Music=false;
                        else Music=true;
                    }
                    if (SDL_PointInRect(&MouseClick,&SoundButtonLocation)) Volume++;
                    if (Volume==3) Volume=0;
                    updateVolume();
                }
                Audio();
                SDL_RenderPresent(renderer);
            }
        void paused()
            {
                SDL_Event event;
                SDL_PollEvent(&event);
                SoundButtonLocation = {680,400,128,128};
                MusicButtonRect = {380,400,128,128};
                renderTexture(DeathBG,360,220,480,360,renderer,NULL);
                write("Paused",400,300);
                SoundButtonRect = {Volume*16,0,16,16};
                if (Music) renderTexture(MusicButtonOn,380,400,128,128,renderer,NULL);
                else renderTexture(MusicButtonOff,380,400,128,128,renderer,NULL);
                renderTexture(PauseButton,530,400,128,128,renderer,NULL);
                renderTextureAt(SoundButton,SoundButtonRect,SoundButtonLocation);
                if (event.type==SDL_MOUSEBUTTONDOWN)
                {
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Point MouseClick ={x,y};
                    SDL_Rect PauseButtonRect = {530,400,128,128};
                    if (SDL_PointInRect(&MouseClick,&PauseButtonRect))
                    {
                        playSound(menuClick);
                        GameState=S_Game;
                    }
                    if (SDL_PointInRect(&MouseClick,&MusicButtonRect))
                    {
                        playSound(menuClick);
                        if (Music) Music=false;
                        else Music=true;
                    }
                    if (SDL_PointInRect(&MouseClick,&SoundButtonLocation)) Volume++;
                    if (Volume==3) Volume=0;
                    updateVolume();
                    Audio();

                }
                SDL_RenderPresent(renderer);

            }
        void run()
            {
                SpawnMeteors();
                HandleEvents();
                Update();
                Render();
                SDL_Delay(8);
                if (Player.active==false) Death();
            }
        void startNew()
            {
                    setup();
                    StartTime = SDL_GetTicks();
                    Point = 0;

            }
};

#endif // _Engine__H
