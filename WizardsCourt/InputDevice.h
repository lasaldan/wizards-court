//
//  InputDevice.h
//  WizardsCourt
//
//  Created by Daniel Fuller on 11/13/15.
//  Copyright (c) 2015 Daniel Fuller. All rights reserved.
//

#ifndef __WizardsCourt__InputDevice__
#define __WizardsCourt__InputDevice__

#include <stdio.h>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

#define HORIZONTAL_AXIS 0
#define VERTICAL_AXIS 1

#define DPAD_NO_DIRECTION 0
#define DPAD_UP 1
#define DPAD_UP_RIGHT 2
#define DPAD_RIGHT 3
#define DPAD_DOWN_RIGHT 4
#define DPAD_DOWN 5
#define DPAD_DOWN_LEFT 6
#define DPAD_LEFT 7
#define DPAD_UP_LEFT 8

#define LEFT_JOY_H 0
#define LEFT_JOY_V 1
#define RIGHT_JOY_H 2
#define RIGHT_JOY_V 3

class InputDevice {
public:
    InputDevice();
    void PollInputs();
    bool isButtonDown(int);
    int leftStick(int);
    int rightStick(int);
    int dpadDirection();
    void HandleCurrentEvent();
    void SetDevice(SDL_Joystick*);
    bool ShouldQuit();
    void print();
private:
    SDL_Joystick *gamepad;
    SDL_Event event;
    std::vector<bool> buttonStates;
    int dpad;
    int leftXAxis;
    int leftYAxis;
    int rightXAxis;
    int rightYAxis;
    bool quit;
    
    void setLeftJoyValues();
    void setRightJoyValues();
    void setDpadValue();
    void pressButton();
    void releaseButton();
};

#endif /* defined(__WizardsCourt__InputDevice__) */
