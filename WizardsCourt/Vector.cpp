//
//  Vector.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 10/7/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "Vector.h"

Vector::Vector() {
    x = 0;
    y = 0;
    z = 0;
}

Vector::Vector(float X, float Y, float Z) {
    x = X;
    y = Y;
    z = Z;
}

Vector::Vector(Vertex v) {
    Vector(v.x, v.y, v.z);
}

Vector Vector::Cross(Vector left, Vector right) {
    Vector temp = Vector();
    temp.x = left.y*right.z - left.z*right.y;
    temp.y = left.z*right.x - left.x*right.z;
    temp.z = left.x*right.y - left.y*right.x;
    return temp;
}

Vector Vector::Minus(Vector left, Vector right) {
    Vector temp = Vector();
    temp.x = left.x - right.x;
    temp.y = left.y - right.y;
    temp.z = left.z - right.z;
    return temp;
}

Vector Vector::Normalize(Vector v) {
    Vector temp = Vector();
    float length = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    temp.x = v.x / length;
    temp.y = v.y / length;
    temp.z = v.z / length;
    return temp;
}