#include "common_parameters.h"
#include <SDL_mixer.h>

void PlayMusic(const char filename[], Audio* audio, int volume)
{
    if (!audio->musicIsPlaying)
    {
        audio->music = Mix_LoadMUS(filename);

        if (!audio->music)
            printf("%s\n", Mix_GetError());

        Mix_VolumeMusic(255 * volume / 5);

        if (Mix_PlayMusic(audio->music, -1) < 0)
            printf("%s\n", Mix_GetError());

        audio->musicIsPlaying = true;
    }
}

void StopMusic(Audio* audio) {
    audio->musicIsPlaying = false;
    Mix_HaltMusic();
}

void StopSound(Audio* audio, int channel) {
    audio->soundIsPlaying = false;
    Mix_HaltChannel(channel);
}

void EndSound(Audio* audio) {
    audio->soundIsPlaying = false;
}

void PlaySound(const char filename[], int channel, Audio* audio, int volume, int timeFlag)
{
    if (!audio->soundIsPlaying)
    {
        audio->sound = Mix_LoadWAV(filename);
        if (!audio->sound)
            printf("%s\n", Mix_GetError());

        Mix_VolumeChunk(audio->sound, 255 * volume / 5);

        if (Mix_PlayChannel(channel, audio->sound, timeFlag) < 0)
            printf("%s\n", Mix_GetError());

        audio->soundIsPlaying = true;
    }
}