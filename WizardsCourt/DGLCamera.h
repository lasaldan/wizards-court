//
//  DGLCamera.h
//  wizards-court
//
//  Created by Daniel Fuller on 10/9/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__DGLCamera__
#define __wizards_court__DGLCamera__

#include <iostream>
#include <stdio.h>
#include "Vertex.h"
#include "Vector.h"

class DGLCamera {
public:
    DGLCamera();
    
    Vertex position;
    Vertex lookat;
    Vector up;
    
    float rotationX;
    float rotationY;
    
    void rotateY(float amount);
    void rotateX(float amount);
    
    void translateX(float distance);
    void translateY(float distance);
    void translateZ(float distance);
    
    void setPosition(Vertex);
    void setLookat(Vertex);
};

#endif /* defined(__wizards_court__DGLCamera__) */
