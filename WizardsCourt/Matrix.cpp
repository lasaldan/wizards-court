//
//  Matrix.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 9/30/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Matrix.h"
using namespace std;

Matrix::Matrix() {
    MatrixData.resize(4);
    MatrixData[0].resize(4);
    MatrixData[1].resize(4);
    MatrixData[2].resize(4);
    MatrixData[3].resize(4);
}

Matrix::Matrix(vector< vector< float > > matrixData) {
    MatrixData = matrixData;
}

vector< vector< float > > Matrix::GetData() {
    return MatrixData;
}

Matrix Matrix::Transpose(Matrix input) {
    vector< vector<float> > temp;
    temp.resize(4);
    temp[0].resize(4);
    temp[1].resize(4);
    temp[2].resize(4);
    temp[3].resize(4);
    
    for(int row=0; row < temp[0].size(); row ++) {
        for(int col=0; col < temp[0].size(); col++) {
            temp[col][row] = input.Get(row,col);
        }
    }
    
    return temp;
}

Matrix Matrix::InvertTranslation(Matrix m) {
    Matrix temp = Matrix::Identity();
    temp.Set(0,3, -(m.Get(0,3)));
    temp.Set(1,3, -(m.Get(1,3)));
    temp.Set(2,3, -(m.Get(2,3)));
    return temp;
}

Matrix Matrix::Multiply(Matrix input) {
    vector< vector< float > > temp;
    temp.resize(4);
    temp[0].resize(4);
    temp[1].resize(4);
    temp[2].resize(4);
    temp[3].resize(4);
    vector< vector< float > > rightSide = input.GetData();
    // Assume square matrices
    for(int answerRow=0; answerRow < MatrixData[0].size(); answerRow ++) {
        for(int answerCol=0; answerCol < MatrixData[0].size(); answerCol++) {
            float val = 0;
            for(int valCounter=0; valCounter < MatrixData[0].size(); valCounter ++) {
                val += (float)MatrixData[answerRow][valCounter] * (float)rightSide[valCounter][answerCol];
            }
            temp[answerRow][answerCol] = val;
        }
    }
    return temp;
}

float Matrix::Get(int row, int col) {
    return MatrixData[row][col];
}

void Matrix::Set(int row, int col, float val) {
    MatrixData[row][col] = val;
}

Vertex Matrix::Transform(Vertex v) {
    float x = MatrixData[0][0] * v.getX() + MatrixData[0][1] * v.getY() + MatrixData[0][2] * v.getZ() + MatrixData[0][3];
    float y = MatrixData[1][0] * v.getX() + MatrixData[1][1] * v.getY() + MatrixData[1][2] * v.getZ() + MatrixData[1][3];
    float z = MatrixData[2][0] * v.getX() + MatrixData[2][1] * v.getY() + MatrixData[2][2] * v.getZ() + MatrixData[2][3];
    return Vertex(x, y, z);

}

Matrix Matrix::Identity() {
    Matrix identity = Matrix();
    identity.Set(0,0,1);
    identity.Set(1,1,1);
    identity.Set(2,2,1);
    identity.Set(3,3,1);
    return identity;
}