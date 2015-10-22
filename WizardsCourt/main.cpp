//
//  main.cpp
//  WizardsCourt
//
//  Created by Daniel Fuller on 10/21/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "main.h"
#include "Game.h"
using namespace std;


int main (int argc, char **argv)
{
    Game game = Game();
    return game.Run();
}
