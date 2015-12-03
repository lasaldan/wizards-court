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
    lastEvent = 0;
    repeatActionDelay = 3;
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
    
    // resize wizards
    for(int i = 0; i < 16; i++) {
        char numstr[3];
        string base = "body";
        sprintf(numstr, "%d", i);
        string key = base + numstr;
        
        Item& piece = playarea.Get(key);
        piece.scale(.075);
        piece.translateY(.4);
        piece.translateX(.8-.4*(i%4));
        piece.translateZ(.6-.4*(i/4));
    }
    
    Item& board = playarea.Get("board");
    //Item& cube = playarea.Get("cube");
    //Item& body0 = playarea.Get("body0");
    //Item& sun = playarea.Get("sun");
    //Item& body1 = playarea.Get("body1");
    //Item& body2 = playarea.Get("body2");
    //Item& staff = playarea.Get("staff");
    board.scale(.075);
    //board.translateY(.1);
    //sun.scale(.4);
    //sun.translateZ(.3);
    //sun.translateY(.088);
    
    //cube.scale(2);
    //cube.translateY(-.90);
    
    //body0.scale(.075);
    //body0.translateY(.4);
    //body0.translateX(.4);
    
    //body1.scale(.06);
    //body1.translateX(.4);
    
    //body2.scale(.06);
    //body2.translateX(.8);
    
    //staff.scale(.06);
    
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
    
    DGL::setMode( MODEL );
    
    Item& board = playarea.Get("board");
    
    if(gamepad.dpadDirection() == 3 && lastEvent+repeatActionDelay < loopcount) {
        lastEvent = loopcount;
        for(int i = 0; i < 16; i++) {
            selectedPieceIndex = (++selectedPieceIndex % 16);
            if(selectionArea[selectedPieceIndex] != NULL) {
                selectedPiece = selectionArea[selectedPieceIndex];
                selectedPiece -> print();
                break;
            }
        }
    }
    
    if(gamepad.dpadDirection() == 7 && lastEvent+repeatActionDelay < loopcount) {
        lastEvent = loopcount;
        for(int i = 0; i < 16; i++) {
            selectedPieceIndex = (--selectedPieceIndex < 0)? 15 : selectedPieceIndex;
            if(selectionArea[selectedPieceIndex] != NULL) {
                selectedPiece = selectionArea[selectedPieceIndex];
                selectedPiece -> print();
                break;
            }
        }
    }
    
    float boardRotation = gamepad.leftStick(HORIZONTAL_AXIS);
    if( boardRotation > 2000 || boardRotation < -2000) {
        board.rotateY(boardRotation/10000.0);
        
        for(int i = 0; i < 16; i++) {
            char numstr[3];
            string base = "body";
            sprintf(numstr, "%d", i);
            string key = base + numstr;
            
            Item& piece = playarea.Get(key);
            piece.rotateY(boardRotation/10000.0);
        }
    }
    
    //DGL::setMode( MODEL );
    
    //Item& cube = playarea.Get("cube");
    //Item& body0 = playarea.Get("body0");
    //Item& body1 = playarea.Get("body1");
    //Item& body2 = playarea.Get("body2");
    //Item& staff = playarea.Get("staff");
    //board.rotateY(.1);
    //cube.rotateY(.1);
    //body0.rotateY(.1);
    //body1.rotateY(.9);
    //body2.rotateY(.9);
    //staff.rotateY(.3);
    // adjust model
    
    //DGL::setMode( CAMERA );
    
    //DGL::setCameraLocation(Vertex(0,4,4));
    // adjust camera
}

bool
Game::GameOver() {
    
    for(int i = 0; i < 4; i ++) {
        
        // Check columns
        if( board.GetPiece(0,i) != NULL &&
            board.GetPiece(1,i) != NULL &&
            board.GetPiece(2,i) != NULL &&
            board.GetPiece(3,i) != NULL ) {
            if( board.GetPiece(0,i)->getDefinition() &
                board.GetPiece(1,i)->getDefinition() &
                board.GetPiece(2,i)->getDefinition() &
                board.GetPiece(3,i)->getDefinition() )
                return true;
        }
        
        // Check rows
        if( board.GetPiece(i,0) != NULL &&
            board.GetPiece(i,1) != NULL &&
            board.GetPiece(i,2) != NULL &&
            board.GetPiece(i,3) != NULL ) {
            if( board.GetPiece(i,0)->getDefinition() &
                board.GetPiece(i,1)->getDefinition() &
                board.GetPiece(i,2)->getDefinition() &
                board.GetPiece(i,3)->getDefinition() )
                return true;
        }
    }
    
    // Check Diagonal
    if( board.GetPiece(0,0) != NULL &&
        board.GetPiece(1,1) != NULL &&
        board.GetPiece(2,2) != NULL &&
        board.GetPiece(3,3) != NULL ) {
        if( board.GetPiece(0,0)->getDefinition() &
            board.GetPiece(1,1)->getDefinition() &
            board.GetPiece(2,2)->getDefinition() &
            board.GetPiece(3,3)->getDefinition() )
            return true;
    }
    
    if( board.GetPiece(0,3) != NULL &&
        board.GetPiece(1,2) != NULL &&
        board.GetPiece(2,1) != NULL &&
        board.GetPiece(3,0) != NULL ) {
        if( board.GetPiece(0,3)->getDefinition() &
            board.GetPiece(1,2)->getDefinition() &
            board.GetPiece(2,1)->getDefinition() &
            board.GetPiece(3,0)->getDefinition() )
            return true;
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
 * Sets initial camera location
 ************/
void
Game::SetupView() {
    DGL::setMode( CAMERA );
//    DGL::translateX(0);
//    DGL::translateY(1);
//    DGL::translateZ(1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 4.0/3.0, .001, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 2.5,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
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