//
//  Face.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/16/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Face.h"


Face::Face() {}

void Face::addVertex(Vertex v) {
    vertices.push_back(v);
}

void Face::addTextureCoordinate(TextureCoordinate t){
    textureCoordinates.push_back(t);
}

void Face::addNormal(Vector v) {
    normals.push_back(v);
}