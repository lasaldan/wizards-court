//
//  vertex.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/15/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "Vertex.h"
using namespace std;

Vertex::Vertex ( float X, float Y, float Z ) {
    x = X;
    y = Y;
    z = Z;
};
Vertex::Vertex ( vector < float > input) {
    x = input[0];
    y = input[1];
    z = input[2];
}
Vertex::Vertex() {
    x = 0;
    y = 0;
    z = 0;
}

float Vertex::getX() {
    return x;
}

float Vertex::getY() {
    return y;
}

float Vertex::getZ() {
    return z;
}

float Vertex::distanceFrom(Vertex v) {
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;
    
    return sqrt( dx*dx + dy*dy + dz*dz );
}

string Vertex::print() {
    ostringstream ss;
    ss << "Ver: [" << x << "," << y << "," << z << "]";
    return ss.str();
}