//
// Created by Miantao on 14/08/2024.
//

#include "game.h"
#include "gamepad.h"


Keys Gamepad::sdl_to_keys(Uint8 button) {
    Keys output;
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A :
            output = Keys::a;
            break;
        case SDL_CONTROLLER_BUTTON_B :
            output = Keys::b;
            break;
        case SDL_CONTROLLER_BUTTON_X :
            output = Keys::x;
            break;
        case SDL_CONTROLLER_BUTTON_Y :
            output = Keys::y;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP :
            output = Keys::up;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN :
            output = Keys::down;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT :
            output = Keys::left;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT :
            output = Keys::right;
            break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER :
            output = Keys::left_button;
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER :
            output = Keys::right_button;
            break;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK :
            output = Keys::left_stick;
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK :
            output = Keys::right_stick;
            break;
        case SDL_CONTROLLER_BUTTON_BACK :
            output = Keys::back;
            break;
        case SDL_CONTROLLER_BUTTON_START :
            output = Keys::start;
            break;
        default :
            output = Keys::other;
            break;
    }
    return output;
}

Gamepad::Gamepad(int joystick_index) {
    this->joystick_index = joystick_index;
    this->pGameController = SDL_GameControllerOpen(joystick_index);

    std::unordered_set<Keys> all_keys({Keys::a, Keys::b, Keys::x, Keys::y, Keys::up, Keys::down, Keys::left, Keys::right, Keys::back, Keys::start, Keys::left_stick, Keys::right_stick, Keys::left_button, Keys::right_button, Keys::left_trigger, Keys::right_trigger, Keys::none, Keys::other});

    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(pressedMap, pressedMap.end()), [](const Keys &key){return std::pair(key, false);});
    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(downMap, downMap.end()), [](const Keys &key){return std::pair(key, false);});
    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(upMap, upMap.end()), [](const Keys &key){return std::pair(key, false);});
    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(downTimeMap, downTimeMap.end()), [](const Keys &key){return std::pair(key, 0.0);});

    leftTrigger = 0;
    rightTrigger = 0;
    leftStick = r2::Vec(0,0);
    rightStick = r2::Vec(0,0);

    triggerDeadzone_ = 0.7;
}

void Gamepad::reset() {
    for (auto & item : upMap)
        item.second = false;

    for (auto & item : downMap)
        item.second = false;
}

void Gamepad::keyDown(const Keys &key, double time) {
    //the keyboard recognises holding a inputKey as multiple downs without up. To filter these out, if a inputKey is already pressed, it can't go down a second time.
    downMap[key] = not pressedMap[key];
    pressedMap[key] = true;
    downTimeMap[key] = time;
}

void Gamepad::keyUp(const Keys &key) {
    upMap[key] = true;
    pressedMap[key] = false;
}

void Gamepad::axisMotion(Uint8 axis, Sint16 value) {
    //todo clean this up
    double valueDouble = std::max(-1.0, value/32767.0); //not allowed to go below 1, smallest value is -32768 < -32767, so this has to be eliminated.
    bool new_state;

    switch (axis) {
        case SDL_CONTROLLER_AXIS_LEFTX :
            leftStick.x = valueDouble;
            break;
        case SDL_CONTROLLER_AXIS_RIGHTX :
            rightStick.x = valueDouble;
            break;
        case SDL_CONTROLLER_AXIS_RIGHTY :
            rightStick.y = - valueDouble; //y-axis is inverted
            break;
        case SDL_CONTROLLER_AXIS_LEFTY :
            leftStick.y = - valueDouble; //y-axis is inverted
            break;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT :
            rightTrigger = valueDouble;
            new_state = (valueDouble > triggerDeadzone_);
            if (pressedMap[Keys::right_trigger] != new_state) {
                if (new_state) {
                    downMap[Keys::right_trigger] = true;
                } else {
                    upMap[Keys::right_trigger] = false;
                }
            }
            pressedMap[Keys::right_trigger] = new_state;
            break;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT :
            leftTrigger = valueDouble;
            new_state = (valueDouble > triggerDeadzone_);
            if (pressedMap[Keys::left_trigger] != new_state) {
                if (new_state) {
                    downMap[Keys::left_trigger] = true;
                } else {
                    upMap[Keys::left_trigger] = false;
                }
            }
            pressedMap[Keys::left_trigger] = new_state;
            break;
        default :
            break;
    }
}

void Gamepad::close() {
    SDL_GameControllerClose(pGameController);
}