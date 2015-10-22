//
//  Matrix.h
//  wizards-court
//
//  Created by Daniel Fuller on 9/30/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__Matrix__
#define __wizards_court__Matrix__

#include <stdio.h>
#include <vector>
#include <math.h>       /* sin */
#include "Vertex.h"

#define PI 3.14159265358979

class Matrix {
private:
    std::vector< std::vector< float > > MatrixData;
public:
    Matrix();
    Matrix(std::vector< std::vector< float > > matrixData);
    Matrix Multiply(Matrix);
    Matrix static Transpose(Matrix);
    std::vector< std::vector< float > > GetData();
    float Get(int, int);
    void Set(int, int, float);
    Vertex Transform(Vertex);
    static Matrix Identity();
    Matrix static InvertTranslation(Matrix m);
};

#endif /* defined(__wizards_court__Matrix__) */
