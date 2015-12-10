//
//  SoundPlayer.h
//  WizardsCourt
//
//  Created by Daniel Fuller on 12/9/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __WizardsCourt__SoundPlayer__
#define __WizardsCourt__SoundPlayer__

#include <stdio.h>
#include <SDL2/SDL.h>
#include "SDL2_mixer/SDL_mixer.h"
#include <string>

class SoundPlayer {
private:
    //The music that will be played
    Mix_Music *_backgroundMusic = NULL;
    
    //The sound effects that will be used
    Mix_Chunk *_tick = NULL;
    Mix_Chunk *_select = NULL;
    Mix_Chunk *_gameover = NULL;
    
public:
    SoundPlayer();
    bool loadMedia();
    void startMusic();
    void tick();
    void select();
    void gameover();
};


#endif /* defined(__WizardsCourt__SoundPlayer__) */
