//
// Created by Miantao on 14/08/2024.
//

#ifndef SDL_GAME1_GAMEPAD_H
#define SDL_GAME1_GAMEPAD_H

#include<SDL.h>
#include<SDL_gamecontroller.h>
#include<unordered_map>
#include<unordered_set>
#include"r2.h"


enum class Keys;

class Gamepad {
public:
    static Keys sdl_to_keys(Uint8 button);

    int joystick_index;
    SDL_GameController *pGameController;

    std::unordered_map<Keys, bool> pressedMap;
    std::unordered_map<Keys, bool> upMap;
    std::unordered_map<Keys, bool> downMap;
    std::unordered_map<Keys, double> downTimeMap;

    double leftTrigger;
    double rightTrigger;

    r2::Vec leftStick;
    r2::Vec rightStick;

    Gamepad() = default;

    explicit Gamepad(int joystick_index);

    void reset();

    void keyDown(const Keys& key, double time);

    void keyUp(const Keys& key);

    void axisMotion(Uint8 axis, Sint16 value);

    void close();

private:
    double triggerDeadzone_;
};

#endif //SDL_GAME1_GAMEPAD_H
