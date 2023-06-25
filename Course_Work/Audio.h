#pragma once
#include "common_parameters.h"
#include <SDL_mixer.h>

void PlayMusic(const char filename[], Audio* audio, int volume);
void PlaySound(const char filename[], int channel, Audio* audio, int volume, int timeFlag);
void StopMusic(Audio* audio);
void StopSound(Audio* audio, int channel);
void EndSound(Audio* audio);
