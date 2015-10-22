//
//  TextureCoordinate.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/15/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TextureCoordinate.h"
using namespace std;

TextureCoordinate::TextureCoordinate ( float X, float Y ) {
    x = X;
    y = Y;
};

float TextureCoordinate::getX() {
    return x;
}

float TextureCoordinate::getY() {
    return y;
}


string TextureCoordinate::print() {
    ostringstream ss;
    ss << "Tex: [" << x << "," << y << "]";
    return ss.str();
}

