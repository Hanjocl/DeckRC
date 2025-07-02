//
// Created by miantao on 26-6-25.
//

#include "behavior.h"

#include <stdexcept>

InputBehavior::InputBehavior(int channel_index, double value, InputMode mode) : channel_index(channel_index), value(value), mode(mode) {}

KeyBehavior::KeyBehavior(int channel_index, double value, const SDL_Keycode &key, InputMode mode) : key(key), InputBehavior(channel_index, value, mode) {}

ButtonBehavior::ButtonBehavior(int channel_index, double value, Uint8 button, Uint16 which, InputMode mode) : button(button), which(which), InputBehavior(channel_index, value, mode) {}

AxisBehavior::AxisBehavior(int channel_index, double value, Uint8 button, Uint16 which, AxisAsButton as_button, double threshold, InputMode mode) : threshold(threshold), as_button(as_button), ButtonBehavior(channel_index, value, button, which, mode) {}

void InputBehavior::operator()(std::vector<ChannelDataType> &channels) const {
    switch (mode) {
        case InputMode::set:
            channels.at(channel_index) = value;
            break;
        case InputMode::increment:
            channels.at(channel_index) += value;
            break;
        case InputMode::toggle:
            channels.at(channel_index) = - channels.at(channel_index) + value;
            break;
        case InputMode::toggle_symmetric:
            channels.at(channel_index) = - channels.at(channel_index);
            break;
        default:
            break;
    }
}

void AxisBehavior::operator()(std::vector<ChannelDataType> &channels, Sint32 value) {
    double value_scaled = value/static_cast<double>(max_value);
    switch (as_button) {
        case AxisAsButton::no:
            channels.at(channel_index) = value_scaled * this->value;
            break;
        case AxisAsButton::down:
            if ((value_scaled-threshold) > 0 and (previous_value_scaled-threshold) < 0) {
                ButtonBehavior::operator()(channels);
            }
            previous_value_scaled = value_scaled;
            break;
        case AxisAsButton::up:
            if ((value_scaled-threshold) < 0 and (previous_value_scaled-threshold) > 0) {
                ButtonBehavior::operator()(channels);
            }
            previous_value_scaled = value_scaled;
            break;
        default:
            break;
    }
}


