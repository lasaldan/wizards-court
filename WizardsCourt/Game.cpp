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
    selectionArea.resize(16);
    selectedPiece = NULL;
    mode = GAME_MODE_SELECTION;
    turn = PLAYER_1;
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
    
    while(Running) {
        
        loopcount++;
        
        gamepad.PollInputs();
        if(gamepad.ShouldQuit())
            Running = false;
        
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
    Item& staff = playarea.Get("staff");
    board.scale(.06);
    body.scale(.06);
    staff.scale(.06);
    
    //body.translateX(.4);

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
    gamepad.SetDevice( SDL_JoystickOpen(0) );
    
    pieces = GeneratePieces();
    for(int i = 0; i < 16; i++) {
        selectionArea[i] = &(pieces[i]);
    }
    
    DGL::init();
    
    return true;
}

void Game::Update() {
    
    if(gamepad.dpadDirection() == 3 && loopcount%2) {
        for(int i = 0; i < 16; i++) {
            selectedPieceIndex = (++selectedPieceIndex % 16);
            if(selectionArea[selectedPieceIndex] != NULL) {
                selectedPiece = selectionArea[selectedPieceIndex];
                selectedPiece -> print();
                break;
            }
        }
    }
    
    if(gamepad.dpadDirection() == 7 && loopcount%2) {
        for(int i = 0; i < 16; i++) {
            selectedPieceIndex = (--selectedPieceIndex < 0)? 15 : selectedPieceIndex;
            if(selectionArea[selectedPieceIndex] != NULL) {
                selectedPiece = selectionArea[selectedPieceIndex];
                selectedPiece -> print();
                break;
            }
        }
    }
    
    DGL::setMode( MODEL );
    
    Item& board = playarea.Get("board");
    Item& body = playarea.Get("body");
    Item& staff = playarea.Get("staff");
    board.rotateY(.3);
    body.rotateY(.3);
    staff.rotateY(.3);
    // adjust model
    
    DGL::setMode( CAMERA );
    
    // adjust camera
}

bool
Game::GameOver() {
    
    for(int i = 0; i < 4; i ++) {
        
        // Check columns
        if( board.GetPiece(0,i) != NULL && board.GetPiece(1,i) != NULL && board.GetPiece(2,i) != NULL && board.GetPiece(3,i) != NULL ) {
            if( board.GetPiece(0,i)->getDefinition() & board.GetPiece(1,i)->getDefinition() & board.GetPiece(2,i)->getDefinition() & board.GetPiece(3,i)->getDefinition() )
                return true;
        }
        
        // Check rows
        if( board.GetPiece(i,0) != NULL && board.GetPiece(i,1) != NULL && board.GetPiece(i,2) != NULL && board.GetPiece(i,3) != NULL ) {
            if( board.GetPiece(i,0)->getDefinition() & board.GetPiece(i,1)->getDefinition() & board.GetPiece(i,2)->getDefinition() & board.GetPiece(i,3)->getDefinition() )
                return true;
        }
    }
    
    return false;
}


/************
 * Creates all Pieces for game
 ************/
std::vector< Piece >
Game::GeneratePieces() {
    vector< Piece > allpieces;
    
    for(int color = 0; color < 2; color++) {
        for(int head = 0; head < 2; head++) {
            for(int weapon = 0; weapon < 2; weapon++) {
                for(int height = 0; height < 2; height++) {
                    allpieces.push_back( Piece(head, weapon, height, color) );
                }
            }
        }
    }
    
    return allpieces;
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