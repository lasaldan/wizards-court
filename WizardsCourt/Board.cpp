
//
//  Board.cpp
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Board.h"

Board::Board() {
    board.resize(4);
    board[0].resize(4);
    board[1].resize(4);
    board[2].resize(4);
    board[3].resize(4);
}

Piece*
Board::GetPiece(int row, int col) {
    return board[row][col];
}

std::pair<int,int>
Board::nextAvailableLocation(std::pair<int,int> current) {
    
    int row = current.first;
    int col = current.second;
    
    for( int i = 1; i <= 16; i++) {
        col ++;
        if(col >= 4) {
            col = 0;
            row++;
        }
        if(row >= 4) {
            row = 0;
        }
        if(board[row][col] == NULL) {
            return std::pair<int,int> (row, col);
        }
    }
    
    // if no other free square was found, return the current one
    return current;
}

std::pair<int,int>
Board::previousAvailableLocation(std::pair<int,int> current) {
    
    int row = current.first;
    int col = current.second;
    
    for( int i = 1; i <= 16; i++) {
        col --;
        if(col < 0) {
            col = 3;
            row--;
        }
        if(row < 0) {
            row = 3;
        }
        if(board[row][col] == NULL) {
            return std::pair<int,int> (row, col);
        }
    }
    
    // if no other free square was found, return the current one
    return current;

}