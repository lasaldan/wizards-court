//
//  TextureCoordinate.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/15/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__TextureCoordinate__
#define __wizards_court__TextureCoordinate__


#include <stdlib.h>
#include <string>

class TextureCoordinate {
private:
public:
    float x;
    float y;
    TextureCoordinate(float, float);
    std::string print();
    float getX();
    float getY();
    float getZ();
};

#endif /* defined(__wizards_court__TextureCoordinate__) */
