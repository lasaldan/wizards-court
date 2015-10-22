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
    tireRotation = 0; // For Project 3
}


/************
 * Starts main game loop
 ************/
int
Game::Run() {
    if(Init() == false) {
        return -1;
    }
    
    LoadAssets("parking_lot", parking_lot);
    InitializeScene();
    
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
    
    //Item& car = parking_lot.Get("car");

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
    if((viewport = SDL_CreateWindow("Parking Lot", SCREEN_LOCATION_X, SCREEN_LOCATION_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 0 | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL) {
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

    // Close the window if the X is clicked
    if (e.type == SDL_QUIT) {
        Running = false;
    }
    
    if (e.type == SDL_KEYUP) {
        Running = false;
    }
    
    
    // Capture Button presses
    else if(e.type == SDL_JOYBUTTONDOWN) {
        //if(e.jbutton.button & GAS_BUTTON)
        //    buttons = (0 | GAS_BUTTON);
    }
    else if(e.type == SDL_JOYBUTTONUP) {
        buttons = 0;
    }
    
    
    // Capture Axis Motion
    else if(e.type == SDL_JOYAXISMOTION) {
        if(e.jaxis.axis == LEFT_JOY_X)
            cameraDX = e.jaxis.value;
        if(e.jaxis.axis == LEFT_JOY_Y)
            cameraDZ = e.jaxis.value;
        if(e.jaxis.axis == RIGHT_JOY_X)
            cameraRY = e.jaxis.value;
        if(e.jaxis.axis == RIGHT_JOY_Y)
            cameraRZ = e.jaxis.value;
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
}

void Game::Update() {
    
    DGL::setMode( MODEL );
    Item& car = parking_lot.Get("car");
    
    if(inputs & DPAD_LEFT && tireRotation > -30) {
        tireRotation -= 3;
        parking_lot.Get("tire_front_driver").rotateY(-3);
        parking_lot.Get("tire_front_passenger").rotateY(-3);
    }
    else if(inputs & DPAD_RIGHT && tireRotation < 30) {
        tireRotation += 3;
        parking_lot.Get("tire_front_driver").rotateY(3);
        parking_lot.Get("tire_front_passenger").rotateY(3);
    }
    
    if(buttons & GAS_BUTTON) {
        //car.translateZ(.1);
        
        float newX = car.translationX;
        float newZ = car.translationZ;
        
        newX = sin(DGL::toRadians(car.rotationY))/30;
        newZ = cos(DGL::toRadians(car.rotationY))/30;
        
        car.translateX( newX );
        car.translateZ( -newZ );
        
        car.rotateY(tireRotation / 20);
        
        UpdateTireLocations();
        

    }
    if(buttons & BRAKE_BUTTON) {
        
        float newX = car.translationX;
        float newZ = car.translationZ;
        
        newX = -sin(DGL::toRadians(car.rotationY))/30;
        newZ = -cos(DGL::toRadians(car.rotationY))/30;
        
        car.translateX( newX );
        car.translateZ( -newZ );
        
        car.rotateY(-tireRotation / 20);
        
        UpdateTireLocations();
    }
    
    
    DGL::setMode( CAMERA );
    
    if(cameraRY > 1024 || cameraRY < -1024) {
        DGL::rotateY(cameraRY / 10000);
    }
    if(cameraDX > 1024 || cameraDX < -1024)
        DGL::translateX( cameraDX / 100000 );
    
    if(cameraDZ > 1024 || cameraDZ < -1024)
        DGL::translateZ( cameraDZ / 100000 );
    
    if(cameraRZ > 1024 || cameraRZ < -1024)
        DGL::rotateX(cameraRZ/100000);
}


/************
 * Cleans up a few SDL resources
 ************/
void
Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DGL::drawScene( parking_lot );
    
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
    DGL::translateZ( 2 );
    DGL::translateX( 0 );
    DGL::translateY( -1 );
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