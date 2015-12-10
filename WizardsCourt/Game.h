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
#include "Board.h"
#include "ManifestParser.h"
#include "ObjParser.h"
#include "InputDevice.h"
#include "DGL.h"
#include "SoundPlayer.h"

#define GAME_MODE_PLACEMENT 0
#define GAME_MODE_SELECTION 1

#define PLAYER_1 1
#define PLAYER_2 -1

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
    int selectedPieceIndex;
    Piece* selectedPiece;
    int loopcount;
    int turn;
    int mode;
    int lastEvent;
    int repeatActionDelay;
    bool gameOver;
    float selectorOffset;
    
    SoundPlayer audio;
    
    SDL_Window* viewport;
    SDL_GLContext context;
    
    Scene playarea;
    Board board;
    std::vector< Piece* > selectionArea;
    std::vector< Piece > pieces;
    std::vector< Piece* > winningPieces;
    InputDevice gamepad;
    
public:
    Game();
    int Run();
    
private:
    bool Init();
    void InitializeScene();
    void Update();
    void Render();
    void Cleanup();
    void SetupView();
    void LoadAssets(std::string, Scene&);
    void setDefaultTextureSettings();
    bool GameOver();
    void NextState();
    std::vector< Piece > GeneratePieces();
    
    void lookAtBoard();
    void lookAtPieces();
    
    void selectNextPiece();
    void selectPrevPiece();
    void selectNextSquare();
    void selectPrevSquare();
    void movePlaques();
    
    std::pair<float, float> BoardPositionToWorld( std::pair<int, int> );
    std::pair<int,int> selectedAvailableSquare;
};

#endif /* defined(__wizards_court__Game__) */
