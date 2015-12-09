//
//  Item.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/15/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__Item__
#define __wizards_court__Item__

#include <stdio.h>
#include <vector>
#include "Vertex.h"
#include "Vector.h"
#include "TextureCoordinate.h"
#include "Face.h"

class Item {
public:
    std::vector<Vertex> vertices;
    std::vector<TextureCoordinate> textureCoordinates;
    std::vector<Face> faces;
    std::vector<Vector> normals;
    
    float rotationX;
    float rotationY;
    float rotationZ;
    float scalationX;
    float scalationY;
    float scalationZ;
    float translationX;
    float translationY;
    float translationZ;
    
    unsigned int texture;
    
    Item();
    Item(const Item &);
    Item& operator=(const Item& other);
    void rotateX(float);
    void rotateY(float);
    void rotateZ(float);
    
    void translate(float, float, float);
    void translateX(float);
    void translateY(float);
    void translateZ(float);
    
    void scale(float);
    void scaleX(float);
    void scaleY(float);
    void scaleZ(float);
    
    void AddVertex(Vertex);
    void AddNormal(Vector);
    void AddFace(Face);
    void AddTextureCoordinate(TextureCoordinate);
    void SetTexture(unsigned int);
    
    std::vector<Vertex> GetVertices();
    std::vector<TextureCoordinate> GetTextureCoordinates();
    std::vector<Face> GetFaces();
    Vertex GetCenter();
};



#endif /* defined(__wizards_court__Item__) */
