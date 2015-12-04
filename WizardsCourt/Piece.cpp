
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
    
    attributes = 0;
    attributes += (head)?HOOD:HAT;
    attributes += (weap)?SWORD:STAFF;
    attributes += (h)?TALL:SHORT;
    attributes += (c)?BLUE:WHITE;
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

int
Piece::attr() {
    return attributes;
}

std::string
Piece::attrStr() {
    std::string r;
    while(attributes!=0) {r=(attributes%2==0 ?"0":"1")+r; attributes/=2;}
    if(r.length() == 7) r = "0" + r;
    return r;
}


void
Piece::print() {
    
    std::cout << getDefinition() << std::endl;
    
    std::cout << "Hat:" << headwear << std::endl;
    std::cout << "Weapon:" << weapon << std::endl;
    std::cout << "Height:" << height << std::endl;
    std::cout << "Color:" << color << std::endl;
     
}