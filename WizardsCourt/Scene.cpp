//
//  Scene.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/22/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Scene.h"
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
using namespace std;

Scene::Scene() {
}


Item& Scene::Get(string i) {
    return Items.at(i);
}


Item& Scene::AddItem(string name, Item& item) {
    Items.insert( pair<string, Item>(name, item) );
    return Items.at(name);
}