//
//  DGL.cpp
//  wizards-court
//
//  Created by Daniel Fuller on 10/6/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#include "DGL.h"
using namespace std;

// Initialize all static members
Matrix DGL::objRotateX;
Matrix DGL::objRotateY;
Matrix DGL::objRotateZ;
Matrix DGL::objScale;
Matrix DGL::objTranslate;
Matrix DGL::viewRotateX;
Matrix DGL::viewRotateY;
Matrix DGL::viewRotateZ;
Matrix DGL::viewScale;
Matrix DGL::viewTranslate;
Matrix DGL::perspective;
Matrix DGL::objTransformation;
Matrix DGL::viewTransformation;
Matrix DGL::objInverseTransformation;
Matrix DGL::viewInverseTransformation;
Matrix DGL::transformation;
Vertex DGL::cameraLocation;
Vertex DGL::lookAtLocation;
Vector DGL::up;
DGLCamera DGL::camera;
int DGL::mode;
bool DGL::objMatrixDirty;
bool DGL::viewMatrixDirty;



/************
 * Initializes DGL since everything's static and private constructor
 ************/
void
DGL::init() {
    objMatrixDirty = true;
    viewMatrixDirty = true;
    
    objRotateX = Matrix::Identity();
    objRotateY = Matrix::Identity();
    objRotateZ = Matrix::Identity();
    objScale = Matrix::Identity();
    objTranslate = Matrix::Identity();
    objTransformation = Matrix::Identity();
    objInverseTransformation = Matrix::Identity();
    
    viewRotateX = Matrix::Identity();
    viewRotateY = Matrix::Identity();
    viewRotateZ = Matrix::Identity();
    viewScale = Matrix::Identity();
    viewTranslate = Matrix::Identity();
    viewTransformation = Matrix::Identity();
    viewInverseTransformation = Matrix::Identity();
    
    perspective = Matrix::Identity();
    
    lookAtLocation = Vertex(0,0,0);
    cameraLocation = Vertex(0,2,8);
    up = Vector(0,1,0);
    
    camera = DGLCamera();
}


/************
 * Sets the mode.
 * 0 = object mode
 * 1 = camera mode
 * Transformation functions modify the matrix that cooresponds to the current mode
 ************/
void
DGL::setMode(int newMode) {
    
    if( newMode != MODEL && newMode > CAMERA )
        return;
    
    mode = newMode;
}


/************
 * Sets current texture
 ************/
void
DGL::setTexture(unsigned int id) {
    glBindTexture(GL_TEXTURE_2D, id);
}


/************
 * Modifies the current mode's translation matrices
 ************/
void
DGL::translateXYZ(float valX, float valY, float valZ) {
    switch (mode) {
        case MODEL:
            translateX(valX);
            translateY(valY);
            translateZ(valZ);
            break;
            
        case CAMERA:
            break;
    }
}


/************
 * Modifies the current mode's translation matrix X value
 ************/
void
DGL::translateX(float val) {
    switch (mode) {
        case MODEL:
            objTranslate.Set(0,3,val);
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            /*
            camera.position.x += val;
            viewMatrixDirty = true;
            */
            Vector from = Vector(cameraLocation.x, cameraLocation.y, cameraLocation.z);
            Vector to = Vector(lookAtLocation.x, lookAtLocation.y, lookAtLocation.z);
            Vector direction = Vector::Normalize( Vector::Cross( Vector::Minus(to, from), up ) );
            
            float shiftX = direction.x * val;
            float shiftZ = direction.z * val;
            
            cameraLocation.x += shiftX;
            cameraLocation.z += shiftZ;
            
            lookAtLocation.x += shiftX;
            lookAtLocation.z += shiftZ;
            
            viewMatrixDirty = true;
            
            break;
    }
}


/************
 * Modifies the current mode's translation matrix Y value
 ************/
void
DGL::translateY(float val) {
    switch (mode) {
        case MODEL:
            objTranslate.Set(1,3,val);
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            viewMatrixDirty = true;
            break;
    }
}


/************
 * Modifies the current mode's translation matrix Z value
 ************/
void
DGL::translateZ(float val) {
    switch (mode) {
        case MODEL:
            objTranslate.Set(2,3,val);
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            //lookAtLocation.z += val;
            //cameraLocation.z += val;
            //viewMatrixDirty = true;
            
            Vector from = Vector(cameraLocation.x, cameraLocation.y, cameraLocation.z);
            Vector to = Vector(lookAtLocation.x, lookAtLocation.y, lookAtLocation.z);
            Vector direction = Vector::Normalize( Vector::Cross( up , Vector::Cross( up, Vector::Minus(to, from) ) ) );
            
            float shiftX = direction.x * val;
            float shiftZ = direction.z * val;
            
            cameraLocation.x += shiftX;
            cameraLocation.z += shiftZ;
            
            lookAtLocation.x += shiftX;
            lookAtLocation.z += shiftZ;
            
            viewMatrixDirty = true;
            cout << "Camera At: " << cameraLocation.x << " " << cameraLocation.y << " " << cameraLocation.z << endl;
             
            break;
    }
}


/************
 * Modifies the current mode's rotation matrices
 ************/
void
DGL::rotateXYZ(float degX, float degY, float degZ) {
    switch (mode) {
        case MODEL:
            rotateX(degX);
            rotateY(degY);
            rotateZ(degZ);
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            break;
    }
}


/************
 * Modifies the current mode's rotateX matrix
 ************/
void
DGL::rotateX(float degrees) {
    switch (mode) {
        case MODEL:
            objRotateX.Set(1, 1, cos( toRadians(degrees) ));
            objRotateX.Set(1, 2, sin( toRadians(degrees) ));
            objRotateX.Set(2, 1, -sin( toRadians(degrees) ));
            objRotateX.Set(2, 2, cos( toRadians(degrees) ));
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            if((lookAtLocation.y > -10 && degrees < 0 ) ||
               (lookAtLocation.y < 10 && degrees > 0))
                lookAtLocation.y += degrees/2;
            viewMatrixDirty = true;
            break;
    }
}


/************
 * Rotates the orbiter point arond the center point by the given amount of degrees (xz plane)
 ************/
Vertex
DGL::rotateAbout(Vertex center, Vertex orbiter, float degrees) {
    Vertex temp = Vertex(orbiter.x, orbiter.y, orbiter.z);
    temp.x = cos(toRadians(degrees)) * (orbiter.x - center.x) - sin(toRadians(degrees)) * (orbiter.z - center.z) + center.x;
    temp.z = sin(toRadians(degrees)) * (orbiter.x - center.x) + cos(toRadians(degrees)) * (orbiter.z - center.z) + center.z;
    return temp;
}


/************
 * Modifies the current mode's rotateY matrix
 ************/
void
DGL::rotateY(float degrees) {
    switch (mode) {
        case MODEL:
            objRotateY.Set(0, 0, cos( toRadians(degrees) ));
            objRotateY.Set(0, 2, -sin( toRadians(degrees) ));
            objRotateY.Set(2, 0, sin( toRadians(degrees) ));
            objRotateY.Set(2, 2, cos( toRadians(degrees) ));
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            /*
            viewRotateY.Set(0, 0, cos( toRadians(degrees) ));
            viewRotateY.Set(0, 2, -sin( toRadians(degrees) ));
            viewRotateY.Set(2, 0, sin( toRadians(degrees) ));
            viewRotateY.Set(2, 2, cos( toRadians(degrees) ));
            viewMatrixDirty = true;
            */
            lookAtLocation = rotateAbout(cameraLocation, lookAtLocation, degrees);
            /*
            float atX = lookAtLocation.x;
            float atZ = lookAtLocation.z;
            float fromX = cameraLocation.x;
            float fromZ = cameraLocation.z;
            
            lookAtLocation.x = cos(toRadians(degrees)) * (atX - fromX) - sin(toRadians(degrees)) * (atZ - fromZ) + fromX;
            lookAtLocation.z = sin(toRadians(degrees)) * (atX - fromX) + cos(toRadians(degrees)) * (atZ - fromZ) + fromZ;
             */
            viewMatrixDirty = true;
            
            
            break;
    }
}


/************
 * Modifies the current mode's rotateZ matrix
 ************/
void
DGL::rotateZ(float degrees) {
    switch (mode) {
        case MODEL:
            objRotateZ.Set(0, 0, cos( toRadians(degrees) ));
            objRotateZ.Set(0, 1, sin( toRadians(degrees) ));
            objRotateZ.Set(1, 0, -sin( toRadians(degrees) ));
            objRotateZ.Set(1, 1, cos( toRadians(degrees) ));
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            float atX = lookAtLocation.x;
            float atZ = lookAtLocation.z;
            float fromX = cameraLocation.x;
            float fromZ = cameraLocation.z;
            
            lookAtLocation.x = cos(toRadians(degrees)) * (atX - fromX) - sin(toRadians(degrees)) * (atZ - fromZ) + fromX;
            lookAtLocation.z = sin(toRadians(degrees)) * (atX - fromX) + cos(toRadians(degrees)) * (atZ - fromZ) + fromZ;
            viewMatrixDirty = true;
            break;
    }
    
}


/************
 * Modifies the current mode's scale matrices
 ************/
void
DGL::scaleXYZ(float valX, float valY, float valZ) {
    switch (mode) {
        case MODEL:
            scaleX(valX);
            scaleY(valY);
            scaleZ(valZ);
            break;
            
        case CAMERA:
            break;
    }
}


/************
 * Modifies the current mode's scale matrix X value
 ************/
void
DGL::scaleX(float val) {
    switch (mode) {
        case MODEL:
            objScale.Set(0,0,val);
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            break;
    }
}


/************
 * Modifies the current mode's scale matrix Y value
 ************/
void
DGL::scaleY(float val) {
    switch (mode) {
        case MODEL:
            objScale.Set(1,1,val);
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            break;
    }
}


/************
 * Modifies the current mode's scale matrix Z value
 ************/
void
DGL::scaleZ(float val) {
    switch (mode) {
        case MODEL:
            objScale.Set(2,2,val);
            objMatrixDirty = true;
            break;
            
        case CAMERA:
            break;
    }
}


/************
 * Draws a single face using current pipeline settings
 ************/
void
DGL::drawFace(Face face){
    
    glBegin(GL_POLYGON);
    
    for(int i = 0; i < face.vertices.size(); i ++) {
        glTexCoord2f(face.textureCoordinates[i].x, face.textureCoordinates[i].y);
        Vertex transformed = worldToView( objToWorld( face.vertices[i] ) );
        glVertex3f(transformed.x, transformed.y, transformed.z);
    }
    
    glEnd();
}


/************
 * Draws all faces in vector using current pipeline settings
 ************/
void
DGL::drawFaces(vector<Face> faces){
    
    for (auto& face: faces) {
        drawFace(face);
    }
}


/************
 * Draws all faces in an item using current pipeline settings
 ************/
void
DGL::drawItem(Item item){
    
    for (auto& face: item.faces) {
        drawFace(face);
    }
}


/************
 * Draws all items in the scene using current pipeline settings
 ************/
void
DGL::drawScene(Scene scene){
    
    int preMode = mode;
    mode = 0;
    
    for (auto& item: scene.Items) {
        Item obj = item.second;
        
        setTexture(obj.texture);
        
        rotateXYZ(obj.rotationX, obj.rotationY, obj.rotationZ);
        translateXYZ(obj.translationX, obj.translationY, obj.translationZ);
        scaleXYZ(obj.scalationX, obj.scalationY, obj.scalationZ);
        
        drawItem(obj);
    }
    
    mode = preMode;
}

void
DGL::calculateTransformation() {
    transformation = perspective.Multiply(viewTransformation.Multiply(objTransformation));
}

Vertex
DGL::transform(Vertex v) {
    
    bool transformDirty = false;
    
    if(objMatrixDirty || viewMatrixDirty)
        transformDirty = true;
    
    if(objMatrixDirty)
        calculateObjectTransformation();
    
    if(viewMatrixDirty)
        calculateViewTransformation();
    
    if(transformDirty)
        calculateTransformation();
    
    
    float x = (transformation.Get(0,0) * v.getX() +
               transformation.Get(0,1) * v.getY() +
               transformation.Get(0,2) * v.getZ() +
               transformation.Get(0,3));
    
    float y = (transformation.Get(1,0) * v.getX() +
               transformation.Get(1,1) * v.getY() +
               transformation.Get(1,2) * v.getZ() +
               transformation.Get(1,3));
    
    float z = (transformation.Get(2,0) * v.getX() +
               transformation.Get(2,1) * v.getY() +
               transformation.Get(2,2) * v.getZ() +
               transformation.Get(2,3));
    
    return Vertex(x,y,z);
    
    
}



/************
 * Converts the provided vertex to world coordinates using the current model
 * transformation matrices.
 ************/
Vertex
DGL::viewToPerspective( Vertex v ) {
    
    
    float perspectiveDivisor =
    (perspective.Get(3,0) * v.getX() +
     perspective.Get(3,1) * v.getY() +
     perspective.Get(3,2) * v.getZ() +
     perspective.Get(3,3));
    
    float x = (perspective.Get(0,0) * v.getX() +
               perspective.Get(0,1) * v.getY() +
               perspective.Get(0,2) * v.getZ() +
               perspective.Get(0,3)) / perspectiveDivisor;
    
    float y = (perspective.Get(1,0) * v.getX() +
               perspective.Get(1,1) * v.getY() +
               perspective.Get(1,2) * v.getZ() +
               perspective.Get(1,3)) / perspectiveDivisor;
    
    float z = (perspective.Get(2,0) * v.getX() +
               perspective.Get(2,1) * v.getY() +
               perspective.Get(2,2) * v.getZ() +
               perspective.Get(2,3)) / perspectiveDivisor;
    
    return Vertex(x,y,z);
}


/************
 * Converts the provided vertex to world coordinates using the current model
 * transformation matrices.
 ************/
Vertex
DGL::objToWorld( Vertex v ) {
    
    if(objMatrixDirty)
        calculateObjectTransformation();
    
    float x = (objTransformation.Get(0,0) * v.getX() +
               objTransformation.Get(0,1) * v.getY() +
               objTransformation.Get(0,2) * v.getZ() +
               objTransformation.Get(0,3));
    
    float y = (objTransformation.Get(1,0) * v.getX() +
               objTransformation.Get(1,1) * v.getY() +
               objTransformation.Get(1,2) * v.getZ() +
               objTransformation.Get(1,3));
    
    float z = (objTransformation.Get(2,0) * v.getX() +
               objTransformation.Get(2,1) * v.getY() +
               objTransformation.Get(2,2) * v.getZ() +
               objTransformation.Get(2,3));
    
    return Vertex(x,y,z);
}


/************
 * Converts the provided vertex to view coordinates using the current view
 * transformation matrices.
 ************/
Vertex
DGL::worldToView( Vertex v ) {
    
    if(viewMatrixDirty)
        calculateViewTransformation();
    
    /*
    float perspectiveDivisor = perspective.Get(3,2) * v.z + 1;
    */
    float perspectiveDivisor =
                (perspective.Get(3,0) * v.getX() +
                 perspective.Get(3,1) * v.getY() +
                 perspective.Get(3,2) * v.getZ() +
                 perspective.Get(3,3));
    
    float x =   (viewTransformation.Get(0,0) * v.getX() +
                 viewTransformation.Get(0,1) * v.getY() +
                 viewTransformation.Get(0,2) * v.getZ() +
                 viewTransformation.Get(0,3)) / perspectiveDivisor;
    
    float y =   (viewTransformation.Get(1,0) * v.getX() +
                 viewTransformation.Get(1,1) * v.getY() +
                 viewTransformation.Get(1,2) * v.getZ() +
                 viewTransformation.Get(1,3)) / perspectiveDivisor;
    
    float z =   (viewTransformation.Get(2,0) * v.getX() +
                 viewTransformation.Get(2,1) * v.getY() +
                 viewTransformation.Get(2,2) * v.getZ() +
                 viewTransformation.Get(2,3)) / perspectiveDivisor;
    
    return Vertex(x, y, -z/30); // Invert Z because it's weird
}


/************
 * Sets camera location
 ************/
void
DGL::setCameraLocation(Vertex v) {
    cameraLocation = v;
    viewMatrixDirty = true;
}


/************
 * Sets camera view's look location
 ************/
void
DGL::lookAt(Vertex v) {
    lookAtLocation = v;
    viewMatrixDirty = true;
}


void
DGL::setPerspective() {
    
    float d = cameraLocation.distanceFrom(lookAtLocation);
    //float d = 2;
    cout << "d:" << d << endl;
    
    perspective.Set( 3, 2, -1/d );
    
    /*
    float right = 1;
    float left = -1;
    float top = 1;
    float bottom = -1;
    float near = .001;
    float far = 1000;
    
    
    perspective.Set(0,0, (2 * near) / (right - left));
    perspective.Set(0,2, (right + left) / (right - left));
    
    perspective.Set(1,1, (2 * near) / (top - bottom));
    perspective.Set(1,2, (top + bottom) / (top - bottom));
    
    perspective.Set(2,2, -(far+near)/(far-near));
    perspective.Set(2,3, (-2*far*near)/(far-near));
    
    perspective.Set(3,2,-1);
    perspective.Set(3,3,0);
    */
    
}


/************
 * Converts degrees to radians
 ************/
float
DGL::toRadians(float degrees) {
    return PI * degrees / 180;
}


/************
 * Converts radians to degrees
 ************/
float
DGL::toDegrees(float radians) {
    return radians * 180 / PI;
}


/************
 * Calculates view transformation matrix
 ************/
void
DGL::calculateViewTransformation() {
    
    
    Matrix changeOfBase = Matrix::Identity();
    
    Vector from = Vector(cameraLocation.x, cameraLocation.y, cameraLocation.z);
    Vector to = Vector(lookAtLocation.x, lookAtLocation.y, lookAtLocation.z);
    
    Vector N = Vector::Normalize( Vector::Minus( from, to ));
    Vector U = Vector::Normalize( Vector::Cross( up, N ));
    Vector V = Vector::Normalize( Vector::Cross( N, U ));
    
    viewTranslate.Set( 0, 3, -lookAtLocation.x);
    viewTranslate.Set( 1, 3, -lookAtLocation.y);
    viewTranslate.Set( 2, 3, -lookAtLocation.z);
    
    changeOfBase.Set( 0, 0, U.x);
    changeOfBase.Set( 0, 1, U.y);
    changeOfBase.Set( 0, 2, U.z);
    
    changeOfBase.Set( 1, 0, V.x);
    changeOfBase.Set( 1, 1, V.y);
    changeOfBase.Set( 1, 2, V.z);
    
    changeOfBase.Set( 2, 0, N.x);
    changeOfBase.Set( 2, 1, N.y);
    changeOfBase.Set( 2, 2, N.z);
    
    setPerspective();
    
    viewTransformation = changeOfBase.Multiply( viewTranslate );
    /*
    viewTransformation = Matrix::Identity();
    
    
    viewTranslate.Set(0,3, camera.position.x);
    viewTranslate.Set(1,3, camera.position.y);
    viewTranslate.Set(2,3, camera.position.z);

    viewTransformation = viewTranslate.Multiply(viewRotateZ).Multiply(viewRotateY).Multiply(viewRotateX);

    calculateViewInverseTransformation();
     */
    
    // just in case the camera look or positions changed
    
    viewMatrixDirty = false;
}

void
DGL::calculateViewInverseTransformation() {
    
    viewInverseTransformation =
        Matrix::Transpose(viewRotateZ)
        .Multiply( Matrix::Transpose(viewRotateY) )
        .Multiply( Matrix::Transpose(viewRotateX) )
        .Multiply( Matrix::InvertTranslation( viewTranslate ) );

}


/************
 * Calculates object transformation matrix
 ************/
Matrix
DGL::calculateObjectTransformation() {
    objTransformation = objTranslate.Multiply(objScale).Multiply(objRotateX).Multiply(objRotateY).Multiply(objRotateZ);
    objMatrixDirty = false;
    return objTransformation;
}


/************
 * Calculates object inverse transformation matrix
 ************/
Matrix
DGL::calculateObjectInverseTransformation() {
    return objInverseTransformation;
}

