//
//  Piece.h
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __WizardsCourt__Piece__
#define __WizardsCourt__Piece__

#include "Item.h"
#include <stdio.h>
#include <string>

#define BLUE 128
#define WHITE 64

#define HOOD 32
#define HAT 16

#define SWORD 8
#define STAFF 4

#define SHORT 2
#define TALL 1

class Piece {
private:
    int headwear;
    int weapon;
    int height;
    int color;
    int attributes;
public:
    Piece();
    Piece(int headwear, int weapon, int height, int color);
    Item* model;
    int getHeadwear();
    int getWeapon();
    int getHeight();
    int getColor();
    Item* getModel();
    void setHeadwear(int);
    void setWeapon(int);
    void setHeight(int);
    void setColor(int);
    void setModel(Item*);
    void print();
    void generateAttrString();
    int attr();
    std::string attrStr();
    int getDefinition();
};

#endif /* defined(__WizardsCourt__Piece__) */
