//
//  Board.h
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __WizardsCourt__Board__
#define __WizardsCourt__Board__

#include <stdio.h>
#include <vector>
#include "Piece.h"

class Board {
private:
    std::vector< std::vector<Piece*> > board;
    
public:
    Board();
    void PlacePiece(int, int, Piece*);
    Piece* GetPiece(int, int);
    std::pair<int,int> nextAvailableLocation(std::pair<int,int>);
    std::pair<int,int> previousAvailableLocation(std::pair<int,int>);
};

#endif /* defined(__WizardsCourt__Board__) */
