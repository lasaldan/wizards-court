
//
//  Piece.cpp
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Piece.h"
#include <iostream>

Piece::Piece() {}

Piece::Piece(int head, int weap, int h, int c) {
    headwear = head;
    weapon = weap;
    height = h;
    color = c;
    generateAttrString();
}

void
Piece::generateAttrString() {
    attributes = 0;
    attributes += (headwear)?HOOD:HAT;
    attributes += (weapon)?SWORD:STAFF;
    attributes += (height)?TALL:SHORT;
    attributes += (color)?BLUE:WHITE;
}

int
Piece::getHeadwear() {
    return headwear;
}

int
Piece::getWeapon(){
    return weapon;
}

int
Piece::getHeight() {
    return height;
}

int
Piece::getColor() {
    return color;
}

int
Piece::getDefinition() {
    return (headwear + (weapon << 1) + (height << 2) + (color << 3));
}

void
Piece::setHeadwear(int h) {
    headwear = h;
    generateAttrString();
}

void
Piece::setWeapon(int h) {
    weapon = h;
    generateAttrString();
}

void
Piece::setColor(int h) {
    color = h;
    generateAttrString();
}

void
Piece::setHeight(int h) {
    height = h;
    generateAttrString();
}

int
Piece::attr() {
    return attributes;
}

std::string
Piece::attrStr() {
    std::string r;
    while(attributes!=0) {r=(attributes%2==0 ?"0":"1")+r; attributes/=2;}
    if(r.length() == 7) r = "0" + r;
    std::cout << attributes << "=" << r << std::endl;
    return r;
}


void
Piece::print() {
    
    //std::cout << getDefinition() << std::endl;
    
    std::string h = (height)?"Tall":"Short";
    std::string c = (color)?"Blue":"White";
    std::string w = (weapon)?"Sword":"Staff";
    std::string head = (headwear)?"Hood":"Hat";
    
    std::cout <<  h << " " << c << " with " << head << " and " << w << std::endl;
     
}