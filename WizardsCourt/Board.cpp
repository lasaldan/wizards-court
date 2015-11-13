
//
//  Board.cpp
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Board.h"

Board::Board() {
    
}

Piece*
Board::GetPiece(int row, int col) {
    return board[row][col];
}