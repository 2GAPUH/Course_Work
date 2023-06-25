#include "common_parameters.h"
#include <SDL_mixer.h>

void PlayMusic(const char filename[], Audio* audio, int volume)
{
    if (!audio->isPlaying)
    {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        audio->music = Mix_LoadMUS(filename);

        if (!audio->music)
            printf("%s\n", Mix_GetError());

        Mix_VolumeMusic(255 * volume / 5);

        if (Mix_PlayMusic(audio->music, -1) < 0)
            printf("%s\n", Mix_GetError());

        audio->isPlaying = true;
    }
}

void stopMusic(Audio* audio) {
    audio->isPlaying = false;
    Mix_CloseAudio();
}

void PlaySound(const char filename[], int channel, Audio audio, int volume)
{
    audio.sound = Mix_LoadWAV(filename);
    if (!audio.sound)
        printf("%s\n", Mix_GetError());

    Mix_VolumeChunk(audio.sound, 255*volume/5);

    if (Mix_PlayChannel(channel, audio.sound, 0) < 0)
        printf("%s\n", Mix_GetError());
}