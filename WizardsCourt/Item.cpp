//
//  Item.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/15/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Item.h"
#include <stdio.h>
using namespace std;


Item::Item() {
    rotationX = 0;
    rotationY = 0;
    rotationZ = 0;
    scalationX = 1.0f;
    scalationY = 1.0f;
    scalationZ = 1.0f;
    translationX = 0;
    translationY = 0;
    translationZ = 0;
}

void Item::AddVertex(Vertex v){
    vertices.push_back(v);

}

void Item::rotateX(float degrees) {
    rotationX += degrees;
}

void Item::rotateY(float degrees) {
    rotationY += degrees;
}

void Item::rotateZ(float degrees) {
    rotationZ += degrees;
}

void Item::translate(float x, float y, float z) {
    translateX(x);
    translateY(y);
    translateZ(z);
}

void Item::translateX(float distance) {
    translationX += distance;
}

void Item::translateY(float distance) {
    translationY += distance;
}

void Item::translateZ(float distance) {
    translationZ += distance;
}

Vertex Item::GetCenter() {
    return Vertex(translationX, translationY, translationZ);
}

void Item::scale(float amount) {
    scaleX(amount);
    scaleY(amount);
    scaleZ(amount);
}

void Item::scaleX(float amount) {
    scalationX *= amount;
}

void Item::scaleY(float amount) {
    scalationY *= amount;
}

void Item::scaleZ(float amount) {
    scalationZ *= amount;
}

void Item::AddTextureCoordinate(TextureCoordinate t){
    textureCoordinates.push_back(t);
}

void Item::SetTexture(unsigned int i) {
    texture = i;
}

void Item::AddFace(Face f){
    faces.push_back(f);
}

std::vector<Vertex> Item::GetVertices(){
    return vertices;
}

std::vector<TextureCoordinate> Item::GetTextureCoordinates(){
    return textureCoordinates;
}

std::vector<Face> Item::GetFaces(){
    return faces;
}
