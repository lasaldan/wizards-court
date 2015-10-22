//
//  vertex.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/15/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef wizards_court_vertex_h
#define wizards_court_vertex_h

#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>

class Vertex {
public:
    float x;
    float y;
    float z;
    
    Vertex(std::vector< float >);
    Vertex(float, float, float);
    Vertex();
    
    std::string print();
    float distanceFrom(Vertex);
    
    float getX();
    float getY();
    float getZ();
};

#endif
