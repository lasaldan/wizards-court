//
//  Piece.h
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __WizardsCourt__Piece__
#define __WizardsCourt__Piece__

#include <stdio.h>

#define HAT 0
#define HOOD 1

#define BLUE 0
#define WHITE 1

#define STAFF 0
#define SWORD 1

#define TALL 0
#define SHORT 1

class Piece {
private:
    int headwear;
    int weapon;
    int height;
    int color;
public:
    Piece();
    Piece(int headwear, int weapon, int height, int color);
    int getHeadwear();
    int getWeapon();
    int getHeight();
    int getColor();
    void print();
    int getDefinition();
};

#endif /* defined(__WizardsCourt__Piece__) */
