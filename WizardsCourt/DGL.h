//
//  DGL.h
//  wizards-court
//
//  Created by Daniel Fuller on 10/6/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __wizards_court__DGL__
#define __wizards_court__DGL__

#include <stdio.h>
#include "Matrix.h"
#include "Face.h"
#include "Vector.h"
#include "Vertex.h"
#include "DGLCamera.h"
#include "Scene.h"
#include "Item.h"
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#include <iostream>

#define MODEL 0
#define CAMERA 1

class DGL {
    
private:
    DGL();
    static int mode;
    static DGLCamera camera;
    static Vertex origin;
    
public:
    static void init();
    
    static void setMode(int);
    static void setTexture(unsigned int);
    
    static void translateXYZ(float, float, float);
    static void translateX(float);
    static void translateY(float);
    static void translateZ(float);
    
    static void rotateXYZ(float, float, float);
    static void rotateX(float);
    static void rotateY(float);
    static void rotateZ(float);
    
    static void scaleXYZ(float, float, float);
    static void scaleX(float);
    static void scaleY(float);
    static void scaleZ(float);
    
    static void drawFace(Face);
    static void drawFaces(std::vector<Face>);
    static void drawItem(Item);
    static void drawScene(Scene);
    static Vertex rotateAbout(Vertex, Vertex, float);
    
    static float toRadians(float);
    static float toDegrees(float);
    
private:
    
    static Vertex lookAtLocation;
    static Vertex cameraLocation;
    static Vector up;
    
    static bool objMatrixDirty;
    static bool viewMatrixDirty;
    
    static Matrix objRotateX;
    static Matrix objRotateY;
    static Matrix objRotateZ;
    static Matrix objScale;
    static Matrix objTranslate;
    static Matrix objTransformation;
    static Matrix objInverseTransformation;
    
    static Matrix viewRotateX;
    static Matrix viewRotateY;
    static Matrix viewRotateZ;
    static Matrix viewScale;
    static Matrix viewTranslate;
    static Matrix viewTransformation;
    static Matrix viewInverseTransformation;
    
    static Matrix perspective;
    
    static Matrix transformation;
    
    static Matrix calculateObjectTransformation();
    static Matrix calculateObjectInverseTransformation();
    static void calculateViewTransformation();
    static void calculateViewInverseTransformation();
    
    static void calculateTransformation();
    static Vertex transform(Vertex);
    
    static void setPerspective();
    
    static void lookAt( Vertex );
    static void setCameraLocation( Vertex );
    
    static Vertex objToWorld(Vertex);
    static Vertex worldToView(Vertex);
    static Vertex viewToPerspective(Vertex);
    
};

#endif /* defined(__wizards_court__DGL__) */
