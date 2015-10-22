//
//  Game.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/21/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Game.h"
using namespace std;

int angle;


/************
 * Initializes Game Global Variables
 ************/
Game::Game() {
    viewport = NULL;
    Running = true;
}


/************
 * Starts main game loop
 ************/
int
Game::Run() {
    if(Init() == false) {
        return -1;
    }
    
    LoadAssets("wizardscourt", playarea);
    InitializeScene();
    SetupView();
    
    SDL_Event Event;
    
    while(Running) {
        while(SDL_PollEvent(&Event)) {
            HandleEvent(Event);
        }
        
        Update();
        Render();
    }
    
    Cleanup();
    
    return 0;
}


/************
 * Positions items to their starting locations in the scene
 ************/
void
Game::InitializeScene() {
    
    Item& board = playarea.Get("board");
    Item& body = playarea.Get("body");
    board.scale(.06);
    body.scale(.06);

}


/************
 * Initializes SDL and other general variables for the Game
 ************/
bool
Game::Init() {
    
    
    // Initializes SDL with Sounds, Joystick, Window, etc
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    
    // Create viewport
    if((viewport = SDL_CreateWindow("Wizards Court", SCREEN_LOCATION_X, SCREEN_LOCATION_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 0 | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL) {
        return false;
    }
    
    
    // Create drawing context for the viewport
    context = SDL_GL_CreateContext(viewport);
    
    
    // A couple OPENGL defaults here
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // the first parameters adjust location of viewport in window
    glEnable( GL_DEPTH_TEST );
    
    
    // Initialize gamepad
    SDL_JoystickEventState(SDL_ENABLE);
    gamepad = SDL_JoystickOpen(0);
    
    DGL::init();
    
    return true;
}


/************
 * Captures input from keyboard, mouse and gamepad
 ************/
void
Game::HandleEvent(SDL_Event &e) {
    
    switch( e.type ){
            
        case SDL_KEYDOWN:
        switch( e.key.keysym.sym ) {
            case SDLK_LEFT:
                cout << "LEFT" << endl;
                DGL::setMode(CAMERA);
                DGL::translateX(-.1);
                break;
            case SDLK_RIGHT:
                cout << "RIGHT" << endl;
                DGL::setMode(CAMERA);
                DGL::translateX(.1);
                break;
            case SDLK_UP:
                cout << "UP" << endl;
                break;
            case SDLK_DOWN:
                cout << "DOWN" << endl;
                break;
            default:
                break;
        }
        break;
            
        case SDL_QUIT:
            Running = false;
            break;
    }
    /*
    
    // Capture Button presses
    else if(e.type == SDL_JOYBUTTONDOWN) {
        //if(e.jbutton.button & GAS_BUTTON)
        //    buttons = (0 | GAS_BUTTON);
    }
    else if(e.type == SDL_JOYBUTTONUP) {
        buttons = 0;
    }
    
    
    // Capture Hat Motion
    else if(e.type == SDL_JOYHATMOTION) {

        if(e.jhat.value & SDL_HAT_LEFT) {
            inputs = (0 | DPAD_LEFT);
        }
        
        if(e.jhat.value & SDL_HAT_RIGHT) {
            inputs = (0 | DPAD_RIGHT);
        }
        
        if(e.jhat.value == 0) {
            inputs = 0;
        }
    }
     */
}

void Game::Update() {
    
    DGL::setMode( MODEL );
    
    Item& board = playarea.Get("board");
    Item& body = playarea.Get("body");
    board.rotateY(.3);
    body.rotateY(.3);
    // adjust model
    
    DGL::setMode( CAMERA );
    
    // adjust camera
}


/************
 * Cleans up a few SDL resources
 ************/
void
Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DGL::drawScene( playarea );
    
    SDL_GL_SwapWindow(viewport);
}


/************
 * Cleans up a few SDL resources
 ************/
void
Game::Cleanup() {
    //SDL_GL_DeleteContext(viewport);
    SDL_Quit();
}


/************
 * Cleans up a few SDL resources
 ************/
void
Game::SetupView() {
    DGL::setMode( CAMERA );
}


/************
 * Loads all objects into memory from the manifest file. Stores them in the provided scene
 ************/
void
Game::LoadAssets(string filename, Scene& scene) {
    
    ManifestParser assets = ManifestParser(RESOURCE_ROOT + "manifest/" + filename + ".manifest");
    ObjParser parser = ObjParser();

    map<string, GLuint> textures;

    
    // Load all textures via SOIL and add to textures map
    for (auto& it: assets.GetTextures()) {
        GLuint tex = SOIL_load_OGL_texture((RESOURCE_ROOT + it.second).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        pair<string, GLuint> p = pair<string, GLuint>(it.first, tex);
        textures.insert( p );
    }
    

    // Create all items and assign correct texture.
    // Adds all items to the specified scene
    for (auto& it: assets.GetModels()) {
        Item temp = Item();
        parser.parseFile((RESOURCE_ROOT + it.second), &temp);
        temp.SetTexture(textures.at(it.first));
        scene.AddItem(it.first, temp);
    }
    
    setDefaultTextureSettings();
}


/************
 * Sets default texture settings for mapping in OpenGL
 ************/
void
Game::setDefaultTextureSettings() {
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}