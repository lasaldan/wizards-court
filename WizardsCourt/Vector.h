//
//  Vector.h
//  wizards-court
//
//  Created by Daniel Fuller on 10/7/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__Vector__
#define __wizards_court__Vector__

#include <stdio.h>
#include <math.h>
#include "Vertex.h"

class Vector {
private:
    
    
public:
    Vector();
    Vector(Vertex);
    Vector(float, float, float);
    
    float x;
    float y;
    float z;
    
    static Vector Cross(Vector, Vector);
    static Vector Minus(Vector, Vector);
    static Vector Normalize(Vector);
    
};

#endif /* defined(__wizards_court__Vector__) */
