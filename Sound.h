#ifndef _Sound__H
#define _Sound__H


Mix_Music* MusicAudio;
Mix_Chunk* menuClick;
Mix_Chunk* menuClick2;
Mix_Chunk* fireSound;
Mix_Chunk* deathSound;
Mix_Chunk* upgradeSound;
Mix_Chunk* explosion;
void initSound()
{
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    fireSound = Mix_LoadWAV("sfx/fireSound.wav");
    menuClick = Mix_LoadWAV("sfx/menuClick.wav");
    MusicAudio = Mix_LoadMUS("sfx/Music1.mp3");
    menuClick2 = Mix_LoadWAV("sfx/menuClick2.wav");
    deathSound = Mix_LoadWAV("sfx/DeathSound.wav");
    upgradeSound = Mix_LoadWAV("sfx/UpgradeSound.wav");
    explosion = Mix_LoadWAV("sfx/Explosion.wav");
}
void Audio()
{
    if (!isPlaying and Music)
    {
        Mix_ResumeMusic();
        isPlaying=true;
    }
    if (!Music and isPlaying)
    {
        Mix_PauseMusic();
        isPlaying=false;
    }
}
void updateVolume()
{
    Mix_VolumeMusic(Volume*40);
    Mix_Volume(-1,Volume*40);
}
void playSound(Mix_Chunk* s)
{
    Mix_PlayChannel(-1, s, 0);
}


#endif // _Sound__H
