//
//  SoundPlayer.cpp
//  WizardsCourt
//
//  Created by Daniel Fuller on 12/9/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "SoundPlayer.h"

SoundPlayer::SoundPlayer() {
    Mix_AllocateChannels(16);
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    loadMedia();
    
}

bool
SoundPlayer::loadMedia()
{
    bool success = true;
    
    _backgroundMusic = Mix_LoadMUS( "/Users/Daniel/workspace/WizardsCourt/WizardsCourt/sound/bg.wav" );
    if( _backgroundMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    _tick = Mix_LoadWAV( "/Users/Daniel/workspace/WizardsCourt/WizardsCourt/sound/tick.wav" );
    if( _tick == NULL )
    {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    _select = Mix_LoadWAV( "/Users/Daniel/workspace/WizardsCourt/WizardsCourt/sound/select.wav" );
    if( _select == NULL )
    {
        printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    _gameover = Mix_LoadWAV( "/Users/Daniel/workspace/WizardsCourt/WizardsCourt/sound/cheering.wav" );
    if( _gameover == NULL )
    {
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    return success;
}

void
SoundPlayer::startMusic() {
    Mix_PlayMusic( _backgroundMusic, -1 );
}

void
SoundPlayer::tick() {
    Mix_PlayChannel ( -1, _tick, 0 );
}

void
SoundPlayer::select() {
    Mix_PlayChannel ( -1, _select, 0 );
}

void
SoundPlayer::gameover() {
    Mix_PlayChannel ( -1, _gameover, 0 );
}