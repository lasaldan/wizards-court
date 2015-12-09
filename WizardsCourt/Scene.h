//
//  Scene.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/22/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__Scene__
#define __wizards_court__Scene__

#include <vector>
#include <stdio.h>
#include "Item.h"
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <map>

class Scene {
private:
public:
    Scene();
    std::map<std::string, Item> Items;
    Item& Get(std::string);
    Item& AddItem(std::string, Item&);
};

#endif /* defined(__wizards_court__Scene__) */
