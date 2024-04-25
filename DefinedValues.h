#ifndef _DefinedValues__H
#define _DefinedValues__H



const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "Asteroid";
const int PLAYER_WIDTH = 48;
const int PLAYER_HEIGHT = 48;
const int PROJECTILE_WIDTH = 16;
const int PROJECTILE_HEIGHT = 32;
const int METEOR_WIDTH = 64;
const int METEOR_HEIGHT = 64;
const int PROJECTILE_SPEED = 15;
const int MaxProjectiles=50;
const int MaxMeteors=50;
const int S_Menu=0;
const int S_Game=1;
const int S_Shop=2;
const int S_Setting=3;
const int S_DeathMenu=4;
const int S_Paused=5;

int Player_SPEED = 5;
int METEOR_SPEED = 3;
int METEOR_HEALTH= 2;
TTF_Font* font;

Uint32 COOLDOWN_DURATION = 800;
Uint32 COOLDOWN_METEOR = 3000;
Uint32 StartTime;
Uint32 CurrentTime;
Uint32 TimePassed;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* PlayerTexture = nullptr;
SDL_Texture* ProjectileTexture=nullptr;
SDL_Texture* MeteorTexture=nullptr;
SDL_Texture* PlayButton=nullptr;
SDL_Texture* ExitButton=nullptr;
SDL_Texture* BG=nullptr;
SDL_Texture* ShopButton=nullptr;
SDL_Texture* SettingButton=nullptr;
SDL_Texture* Damage=nullptr;
SDL_Texture* Speed=nullptr;
SDL_Texture* Health=nullptr;
SDL_Texture* FireRate=nullptr;
SDL_Texture* CloseButton=nullptr;
SDL_Texture* SpaceBG=nullptr;
SDL_Texture* Frame=nullptr;
SDL_Texture* IGBG=nullptr;
SDL_Texture* ShopList=nullptr;
SDL_Texture* DeathBG=nullptr;
SDL_Texture* ReplayButton=nullptr;
SDL_Texture* HomeButton=nullptr;
SDL_Texture* Selected=nullptr;
SDL_Texture* UpgradeGray=nullptr;
SDL_Texture* UpgradeWhite=nullptr;
SDL_Texture* Name=nullptr;
SDL_Texture* GameOver=nullptr;
SDL_Texture* SoundButton=nullptr;
SDL_Texture* MusicButtonOn=nullptr;
SDL_Texture* MusicButtonOff=nullptr;
SDL_Texture* PauseButton=nullptr;




SDL_Rect DamageLevelRect;
SDL_Rect SpeedLevelRect;
SDL_Rect HealthLevelRect;
SDL_Rect FireRateLevelRect;
SDL_Rect UpgradeButton;
SDL_Rect DamageLevelLocation = {328,80,96,96};
SDL_Rect SpeedLevelLocation = {328,235,96,96};
SDL_Rect HealthLevelLocation = {328,382,96,96};
SDL_Rect FireRateLevelLocation = {328,532,96,96};
SDL_Rect MusicButtonRect = {400,500,128,128};
SDL_Rect SoundButtonRect = {0,0,16,16};
SDL_Rect SoundButtonLocation = {540,490,128,128};

int GameState = S_Menu;
int MeteorSpawnX=0;
int MeteorSpawnY=0;
int Point;
int DamageLevel;
int SpeedLevel;
int HealthLevel;
int FireRateLevel;
int ShopSelect=0;
int ShopListLocation[4] = {80,235,382,532};
int totalScore;
int highScore;
int Volume =2;

char* DamageCostT[4]={"50","100","500","1000"};
char* SpeedCostT[4] ={"50","100","500","1000"};
char* HealthCostT[4] = {"50","100","500","1000"};
char* FireRateCostT[4]={"50","100","500","1000"};

int DamageCost[4]={50,100,500,1000};
int SpeedCost[4] ={50,100,500,1000};
int HealthCost[4] = {50,100,500,1000};
int FireRateCost[4]={50,100,500,1000};

bool Upgradable=true;
bool Music=true;
bool firecooldown=false;
bool spawncooldown=false;
bool isPlaying=false;

Uint32 cooldownTimerCallbackProjectile(Uint32 interval, void* param) {
        firecooldown = false;
        return 0;
    }

Uint32 cooldownTimerCallbackMeteor(Uint32 interval, void* param){
    spawncooldown = false;
    MeteorSpawnX=0;
    MeteorSpawnY=0;
    return 0;
}






#endif // _DefinedValues__H
