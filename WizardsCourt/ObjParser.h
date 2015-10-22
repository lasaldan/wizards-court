//
//  ObjReader.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/16/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__ObjReader__
#define __wizards_court__ObjReader__

#include <stdio.h>
#include "Item.h"

class ObjParser {
private:
    Item *item;
public:
    ObjParser();
    void parseFile(std::string, Item*);
private:
    Vertex parseVertex(std::string);
    TextureCoordinate parseTextureCoordinate(std::string);
    Face parseFace(std::string);
};

#endif /* defined(__wizards_court__ObjReader__) */
