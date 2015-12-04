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
    
    pieces = GeneratePieces();
    int xpos = 0;
    int ypos = 0;
    int i = 0;
    
    float shortYOffset = .34;
    float tallYOffset = .435;
    float yoffset = 0;
    
    // Position pieces to their initial locations
    for(Piece& piece : pieces) {
        
        string key = "wizard" + piece.attrStr();
        Item& model = playarea.Get(key);
        
        if(piece.getHeight()) {
            model.scale(.075);
            yoffset = tallYOffset;
        }
        else {
            model.scale(.055);
            yoffset = shortYOffset;
        }
        model.rotateY(180);
        
        float modelx = .39*xpos-1.4;
        float modely = .89*ypos + yoffset;
        
        model.translateY(modely);
        model.translateX(modelx);
        model.translateZ(1.6);
        
        i++;
        
        xpos = i % 8;
        ypos = i / 8;
        
        selectionArea.push_back(&piece);
    }
    
    selectedAvailableSquare = board.nextAvailableLocation(std::pair<int,int>(3,3));
    
    
    /*
 Set GL_LIGHT_0's position to something like 45 degrees to the 'vertical'. Coordinate (1,1,0) should work nicely in most cases.
     Set GL_LIGHT_0's Ambient color to 0,0,0,1
     Set GL_LIGHT_0's Diffuse color to 1,1,1,1
     Set GL_LIGHT_0's Specular color to 1,1,1,1
     Set the glLightModel's global ambient to 0.2,0.2,0.2,1 (this is the default).
     Don't set any other glLight or glLightModel options - just let them default.
     Enable GL_LIGHTING and GL_LIGHT_0.
     Enable GL_COLOR_MATERIAL and set glColorMaterial to GL_AMBIENT_AND_DIFFUSE. This means that glMaterial will control the polygon's specular and emission colours and the ambient and diffuse will both be set using glColor.
     Set the glMaterial's Specular colour to 1,1,1,1
     Set the glMaterial's Emission colour to 0,0,0,1
     Set the glColor to whatever colour you want each polygon to basically appear to be. That sets the Ambient and Diffuse to the same value which is what you generally want.
     */
    
//    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
//    GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 0.0 };
//    
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//    
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_DEPTH_TEST);


    
    Item& board = playarea.Get("board");
    Item& shelf = playarea.Get("shelf");
    Item& sun = playarea.Get("sun");
    board.scale(.075);
    shelf.scale(.075);
    shelf.translateZ(1.9);
    shelf.rotateY(180);
    //sun.translateY(.07);
    sun.translateY(.02);
    sun.scale(.4);
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
    
    Item& boardModel = playarea.Get("board");
    Item& sun = playarea.Get("sun");
    
    int dpadDir = gamepad.dpadDirection();
    
    if(dpadDir == 3 && lastEvent+repeatActionDelay < loopcount) {
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
    
    else if(dpadDir == 7 && lastEvent+repeatActionDelay < loopcount) {
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
    
    else if(dpadDir == 1) {
        lookAtPieces();
    }
    else if(dpadDir == 5) {
        lookAtBoard();
    }
    
    if(gamepad.isButtonDown(4) && lastEvent+repeatActionDelay < loopcount) {
        lastEvent = loopcount;
        selectedAvailableSquare = board.previousAvailableLocation(selectedAvailableSquare);
        sun.translationX =.4*selectedAvailableSquare.second - .6;
        sun.translationZ =.4*selectedAvailableSquare.first - .47;
    }
    else if(gamepad.isButtonDown(5) && lastEvent+repeatActionDelay < loopcount) {
        lastEvent = loopcount;
        selectedAvailableSquare = board.nextAvailableLocation(selectedAvailableSquare);
        sun.translationX =.4*selectedAvailableSquare.second - .6;
        sun.translationZ =.4*selectedAvailableSquare.first - .47;
    }
    
    float boardRotation = gamepad.leftStick(HORIZONTAL_AXIS);
    if( boardRotation > 2000 || boardRotation < -2000) {
        boardModel.rotateY(boardRotation/5000.0);
        sun.rotateY(boardRotation/5000.0);
    }
    
    //sun.translateX(.4*selectedAvailableSquare.second);
    //sun.translateZ(.4*selectedAvailableSquare.first);
    
    cout << selectedAvailableSquare.first << "," << selectedAvailableSquare.second << endl;
    
}

bool
Game::GameOver() {
    
    for(int i = 0; i < 4; i ++) {
        
        // Check columns
        if( board.GetPiece(0,i) != NULL &&
            board.GetPiece(1,i) != NULL &&
            board.GetPiece(2,i) != NULL &&
            board.GetPiece(3,i) != NULL ) {
            if( board.GetPiece(0,i)->attr() &
                board.GetPiece(1,i)->attr() &
                board.GetPiece(2,i)->attr() &
                board.GetPiece(3,i)->attr() )
                return true;
        }
        
        // Check rows
        if( board.GetPiece(i,0) != NULL &&
            board.GetPiece(i,1) != NULL &&
            board.GetPiece(i,2) != NULL &&
            board.GetPiece(i,3) != NULL ) {
            if( board.GetPiece(i,0)->attr() &
                board.GetPiece(i,1)->attr() &
                board.GetPiece(i,2)->attr() &
                board.GetPiece(i,3)->attr() )
                return true;
        }
    }
    
    // Check Diagonal
    if( board.GetPiece(0,0) != NULL &&
        board.GetPiece(1,1) != NULL &&
        board.GetPiece(2,2) != NULL &&
        board.GetPiece(3,3) != NULL ) {
        if( board.GetPiece(0,0)->attr() &
            board.GetPiece(1,1)->attr() &
            board.GetPiece(2,2)->attr() &
            board.GetPiece(3,3)->attr() )
            return true;
    }
    
    if( board.GetPiece(0,3) != NULL &&
        board.GetPiece(1,2) != NULL &&
        board.GetPiece(2,1) != NULL &&
        board.GetPiece(3,0) != NULL ) {
        if( board.GetPiece(0,3)->attr() &
            board.GetPiece(1,2)->attr() &
            board.GetPiece(2,1)->attr() &
            board.GetPiece(3,0)->attr() )
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
    
    lookAtPieces();
}


/************
 * Looks at board
 ***********/
void
Game::lookAtBoard() {
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(0.0, 0.5, 2.5,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
}

void
Game::lookAtPieces() {
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(0.0, 0.8, 1.5,
              0.0, 1.0, 0.0,
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
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}