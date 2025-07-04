//
// Created by miantao on 26-6-25.
//

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <vector>
#include <SDL.h>

using ChannelDataType = int;

enum class InputMode {
    set, increment, toggle, toggle_symmetric, SIZE
};

enum class AxisAsButton {
    no, up, down, SIZE
};

class InputBehavior {
public:
    InputBehavior(int channel_index, double value, InputMode mode=InputMode::set);

    void operator() (std::vector<ChannelDataType> &channels) const;

    int channel_index;

    // Getter functions for protected members
    double getValue() const { return value; }
    InputMode getMode() const { return mode; }

protected:
    InputMode mode;
    double value; // = std::numeric_limits<ChannelDataType>::max();
};

class KeyBehavior : public InputBehavior {
public:
    KeyBehavior(int channel_index, double value, const SDL_Keycode &key, InputMode mode=InputMode::set);

    SDL_Keycode key;
};

class ButtonBehavior : public InputBehavior {
public:
    ButtonBehavior(int channel_index, double value, Uint8 button, Uint16 which, InputMode mode=InputMode::set);

    Uint8 button;
    Uint16 which;
};

class AxisBehavior : public ButtonBehavior {
public:
    static constexpr Sint32 max_value = 32767;

    AxisBehavior(int channel_index, double value, Uint8 button, Uint16 which, AxisAsButton as_button=AxisAsButton::no, double threshold = 0, InputMode mode=InputMode::set);

    void operator() (std::vector<ChannelDataType> &channels, Sint32 value);
    AxisAsButton as_button;    // 0 means not digital, +1 means in response to rising signal, -1 means in response to falling signal
    double threshold;

protected:
    // for digital input
    double previous_value_scaled=0;
};



#endif //BEHAVIOR_H
