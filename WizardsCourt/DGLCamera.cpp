//
//  DGLCamera.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 10/9/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "DGLCamera.h"

DGLCamera::DGLCamera() {
    position = Vertex(0.0f, 1.0f ,3.0f);
    lookat = Vertex(0.0f, 0.0f, 0.0f);
    up = Vector(0.0f, 1.0f, 0.0f);
    rotationX = 0.0f;
    rotationY = 0.0f;
}


void
DGLCamera::rotateY(float amount) {
    rotationY += amount;
}

 
void
DGLCamera::rotateX(float amount) {
    rotationX += amount;
}


void
DGLCamera::translateX(float distance) {
    
    Vector from = Vector(position.x, position.y, position.z);
    Vector to = Vector(lookat.x, lookat.y, lookat.z);
    Vector direction = Vector::Normalize( Vector::Cross( Vector::Minus(to, from), up ) );
    
    float shiftX = direction.x * distance;
    float shiftZ = direction.z * distance;
    
    position.x += shiftX;
    position.z += shiftZ;
    
    lookat.x += shiftX;
    lookat.z += shiftZ;
}


void
DGLCamera::translateY(float distance) {
    position.y += distance;
}


void
DGLCamera::translateZ(float distance) {
    position.z += distance;
}


void
DGLCamera::setPosition(Vertex v) {
    position = v;
}


void
DGLCamera::setLookat(Vertex v) {
    lookat = v;
}