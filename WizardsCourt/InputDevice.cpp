//
//  InputDevice.cpp
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//
//  Built for Logitech Gamepad F310 Using SDL
//  Button States are stored in buttonStates[index] with:
//  X = 0
//  A = 1
//  B = 2
//  Y = 3
// LB = 4
// RB = 5
// LT = 6
// RT = 7
// BK = 8
// ST = 9
// LJ = 10
// RJ = 11

#include "InputDevice.h"
#include <iostream>


InputDevice::InputDevice() {
    buttonStates.resize(13);
    dpad = 0;
    leftXAxis = 0;
    leftYAxis = 0;
    rightXAxis = 0;
    rightYAxis = 0;
    quit = false;
}

void
InputDevice::print() {
    std::cout << "Dpad:" << dpad << std::endl;
    std::cout << "left - X:" << leftXAxis << " Y:" << leftYAxis << std::endl;
    std::cout << "left - X:" << rightXAxis << " Y:" << rightYAxis << std::endl;
    std::cout << "--------" << std::endl;
}

bool
InputDevice::isButtonDown(int buttonId) {
    return buttonStates[buttonId];
}

int
InputDevice::leftStick(int axis) {
    if( axis == HORIZONTAL_AXIS )
        return leftXAxis;
    return leftYAxis;
}

int
InputDevice::rightStick(int axis) {
    if( axis == HORIZONTAL_AXIS )
        return rightXAxis;
    return rightYAxis;
}

int
InputDevice::dpadDirection() {
    return dpad;
}

void
InputDevice::PollInputs() {
    
    while(SDL_PollEvent(&event)) {
        HandleCurrentEvent();
    }
}

void
InputDevice::HandleCurrentEvent() {
    
    if(event.type == SDL_JOYHATMOTION)
        setDpadValue();
    
    else if(event.type == SDL_JOYBUTTONDOWN)
        pressButton();
    
    else if(event.type == SDL_JOYBUTTONUP)
        releaseButton();
    
    else if(event.type == SDL_QUIT)
        quit = true;
    
    else if(event.type == SDL_JOYAXISMOTION && (event.jaxis.axis == LEFT_JOY_H || event.jaxis.axis == LEFT_JOY_V))
        setLeftJoyValues();
    
    else if(event.type == SDL_JOYAXISMOTION && (event.jaxis.axis == RIGHT_JOY_H || event.jaxis.axis == RIGHT_JOY_V))
        setRightJoyValues();
}

void
InputDevice::setLeftJoyValues() {
    if( event.jaxis.axis == LEFT_JOY_H )
        leftXAxis = event.jaxis.value;
    
    else if( event.jaxis.axis == LEFT_JOY_V )
        leftYAxis = event.jaxis.value;
}

void
InputDevice::setRightJoyValues() {
    if( event.jaxis.axis == RIGHT_JOY_H )
        rightXAxis = event.jaxis.value;
    
    else if( event.jaxis.axis == RIGHT_JOY_V )
        rightYAxis = event.jaxis.value;
}

bool
InputDevice::ShouldQuit() {
    return quit;
}

void
InputDevice::pressButton() {
    buttonStates[event.jbutton.button] = true;
    std::cout << "button:" << (int)event.jbutton.button << std::endl;
}

void
InputDevice::releaseButton() {
    buttonStates[event.jbutton.button] = false;
}

void
InputDevice::setDpadValue() {
    
    if(event.jhat.value & SDL_HAT_LEFT && event.jhat.value & SDL_HAT_UP)
        dpad = DPAD_UP_LEFT;
    
    else if(event.jhat.value & SDL_HAT_RIGHT && event.jhat.value & SDL_HAT_UP)
        dpad = DPAD_UP_RIGHT;
    
    else if(event.jhat.value & SDL_HAT_RIGHT && event.jhat.value & SDL_HAT_DOWN)
        dpad = DPAD_DOWN_RIGHT;
    
    else if(event.jhat.value & SDL_HAT_LEFT && event.jhat.value & SDL_HAT_DOWN)
        dpad = DPAD_DOWN_LEFT;
    
    else if(event.jhat.value & SDL_HAT_RIGHT)
        dpad = DPAD_RIGHT;
    
    else if(event.jhat.value & SDL_HAT_LEFT)
        dpad = DPAD_LEFT;
    
    else if(event.jhat.value & SDL_HAT_UP)
        dpad = DPAD_UP;
    
    else if(event.jhat.value & SDL_HAT_DOWN)
        dpad = DPAD_DOWN;
    
    else
        dpad = DPAD_NO_DIRECTION;

}

void
InputDevice::SetDevice(SDL_Joystick * joy) {
    gamepad = joy;
}