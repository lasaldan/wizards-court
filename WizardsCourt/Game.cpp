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
    gameOver = false;
    selectorOffset = 0;
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
    for(int i = 0; i < 16; i++) {
        selectionArea[i] = &(pieces[i]);
    }
    
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
    }
    
    selectNextSquare();
    
    Item& board = playarea.Get("board");
    Item& shelf = playarea.Get("shelf");
    Item& sun = playarea.Get("sun");
    board.scale(.075);
    shelf.scale(.075);
    shelf.translateZ(1.9);
    shelf.rotateY(180);
    sun.translateY(.2);
    sun.scale(.15);
}




/************
 * Sets the current state for the Game
 ************/
void
Game::NextState() {
    if(mode == GAME_MODE_SELECTION) {
        mode = GAME_MODE_PLACEMENT;
        turn *= -1;
        lookAtBoard();
    }
    else {
        mode = GAME_MODE_SELECTION;
        lookAtPieces();
    }
    
    std::string m = (mode==GAME_MODE_SELECTION)?"Select a piece":"Place the piece";
    std::string p = (turn==PLAYER_1)?"Player 1":"Player 2";
    
    cout << p << " -> " << m << endl;
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
    
    DGL::init();
    
    return true;
}

void Game::Update() {
    
    DGL::setMode( MODEL );
    
    Item& boardModel = playarea.Get("board");
    Item& sun = playarea.Get("sun");
    
    int dpadDir = gamepad.dpadDirection();
        
    if((dpadDir == 7 || gamepad.isButtonDown(4)) && lastEvent+repeatActionDelay < loopcount) {
        lastEvent = loopcount;
        if(mode == GAME_MODE_SELECTION)
            selectNextPiece();
        else
            selectNextSquare();
        
    }
    else if((dpadDir == 3 || gamepad.isButtonDown(5)) && lastEvent+repeatActionDelay < loopcount) {
        lastEvent = loopcount;
        if(mode == GAME_MODE_SELECTION)
            selectPrevPiece();
        else
            selectPrevSquare();
    }
    
    else if(dpadDir == 1) {
        lookAtPieces();
    }
    else if(dpadDir == 5) {
        lookAtBoard();
    }
    
    float boardRotation = gamepad.leftStick(HORIZONTAL_AXIS);
    if( boardRotation > 2000 || boardRotation < -2000) {
        boardModel.rotateY(boardRotation/5000.0);
        playarea.Get("sun").rotateY(boardRotation/5000.0);
        for(int row = 0; row < 4; row++) {
            for( int col = 0; col < 4; col++) {
                Piece* t = board.GetPiece(row, col);
                if(t != NULL)
                    t->model->rotateY(boardRotation/5000.0);
            }
        }
    }
    
    if(gamepad.isButtonDown(1) && lastEvent+repeatActionDelay*2 < loopcount && !gameOver) {
        lastEvent = loopcount;
        if(mode == GAME_MODE_SELECTION) {
            selectedPiece = selectionArea[selectedPieceIndex];
            selectedPiece->model->translationX = -.9;
            selectedPiece->model->translationZ = -.4;
            selectedPiece->model->translationY = .69;
            selectedPiece->model->rotationX = -10;
            selectedPiece->model->rotationY = 190;
            selectedPiece->model->scale(.5);
            selectionArea[selectedPieceIndex] = NULL;
        }
        else {
            board.PlacePiece(selectedAvailableSquare.first, selectedAvailableSquare.second, selectedPiece);
            pair<float, float> loc = BoardPositionToWorld(selectedAvailableSquare);

            selectedPiece->model->translationX = loc.first;
            selectedPiece->model->translationZ = loc.second;
            selectedPiece->model->translationY = .25;
            if(selectedPiece->getHeight()) {
                selectedPiece->model->translationY += .1;
            }
            selectedPiece->model->rotationX = 0;
            selectedPiece->model->rotationY = boardModel.rotationY;
            selectedPiece->model->scale(2);
            
            if((gameOver = GameOver())) {
                cout << "GAME OVER" << endl;
                if(turn == PLAYER_1)
                    cout << "PLAYER 1 WINS!!" << endl;
                else
                    cout << "PLAYER 2 WINS!!" << endl;
            }
            
            selectNextSquare();
            selectNextPiece();
        }
        NextState();
    }
    
    selectorOffset = sin((float)loopcount / 10) / 12;
    sun.translationY = .2 + selectorOffset;
    
}

void
Game::selectNextPiece() {
    for(int i = 0; i < 16; i++) {
        selectedPieceIndex = (++selectedPieceIndex % 16);
        if(selectionArea[selectedPieceIndex] != NULL) {
            selectedPiece = selectionArea[selectedPieceIndex];
            cout << "HIGHLIGHTED:" << endl;
            selectedPiece -> print();
            cout << endl;
            break;
        }
    }
}

void
Game::selectPrevPiece() {
    for(int i = 0; i < 16; i++) {
        selectedPieceIndex = (--selectedPieceIndex < 0)? 15 : selectedPieceIndex;
        if(selectionArea[selectedPieceIndex] != NULL) {
            selectedPiece = selectionArea[selectedPieceIndex];
            cout << "HIGHLIGHTED:" << endl;
            selectedPiece -> print();
            cout << endl;
            break;
        }
    }
}

void
Game::selectNextSquare() {
    selectedAvailableSquare = board.previousAvailableLocation(selectedAvailableSquare);
    
    pair<float, float> loc = BoardPositionToWorld(selectedAvailableSquare);
    
    playarea.Get("sun").translationX = loc.first;
    playarea.Get("sun").translationZ = loc.second;
}

void
Game::selectPrevSquare() {
    selectedAvailableSquare = board.nextAvailableLocation(selectedAvailableSquare);
    
    pair<float, float> loc = BoardPositionToWorld(selectedAvailableSquare);
    
    playarea.Get("sun").translationX = loc.first;
    playarea.Get("sun").translationZ = loc.second;
}

pair<float, float>
Game::BoardPositionToWorld( pair<int, int> location ) {
    float x = 0;
    float z = 0;
    
    float betweenSquareCenters = .44;
    
    x = location.first * betweenSquareCenters - betweenSquareCenters*1.5;
    z = location.second * betweenSquareCenters - betweenSquareCenters*1.5;
    
    return pair<float,float> (x, z);
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
                    Piece p = Piece();
                    p.setHeadwear(head);
                    p.setWeapon(weapon);
                    p.setHeight(height);
                    p.setColor(color);
                    p.setModel( &(playarea.Get("wizard" + p.attrStr() )) );
                    allpieces.push_back( p );
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
    gluPerspective(50.0, 4.0/3.0, .5, 4.5);
    
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
        int texId = textures.at(it.first);
        temp.SetTexture(texId);
//        temp.scale(1);
//        temp.translate(0,0,0);
//        temp.rotateX(0);
//        temp.rotateY(0);
//        temp.rotateZ(0);
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
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    // Lighting
    GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
    
    GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
    GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
    // global normal
    //glNormal3f(0.0, 0.0, 1.0);

    // Light 0 = piece selection indicator
    // Light 1 = board illumination spot
    GLfloat lightColor0[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos0[] = {0.0f, -0.3f, 0.0f, 1.0f};
    
    GLfloat lightPos1[] = {0.0f, -0.5f, 0.0f, 1.0f};
    GLfloat boardPos[] = {0.0f, -.5f, 0.0f};
    GLfloat origin[] = {0.0f, 0.0f, 1.0f};
    
    
    glLightf(  GL_LIGHT0, GL_SPOT_CUTOFF,    38.0 );
    glLightf(  GL_LIGHT0, GL_SPOT_EXPONENT,  2.0 );
    //glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, boardPos );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,        lightColor0 );
    glLightfv( GL_LIGHT0, GL_POSITION,       lightPos0 );
    
//    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 50.0);
//    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
//    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, origin);
//    
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor0);
//    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

}