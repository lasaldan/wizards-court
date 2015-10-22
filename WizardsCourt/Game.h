//
//  Game.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/21/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__Game__
#define __wizards_court__Game__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include "SOIL.h"
#include "Scene.h"
#include "Item.h"
#include "ManifestParser.h"
#include "ObjParser.h"
#include "DGL.h"

#define DPAD_LEFT 1
#define DPAD_RIGHT 2

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_LOCATION_X = 50;
const int SCREEN_LOCATION_Y = 50;

const std::string RESOURCE_ROOT = "/Users/Daniel/workspace/WizardsCourt/WizardsCourt/";

class Game {
    
private:
    int inputs;
    int buttons;
    bool Running;
    
    SDL_Window* viewport;
    SDL_GLContext context;
    
    Scene playarea;
    SDL_Joystick *gamepad;
    
public:
    Game();
    int Run();
    
private:
    bool Init();
    void InitializeScene();
    void HandleEvent(SDL_Event&);
    void Update();
    void Render();
    void Cleanup();
    void SetupView();
    void LoadAssets(std::string, Scene&);
    void setDefaultTextureSettings();
};

#endif /* defined(__wizards_court__Game__) */
