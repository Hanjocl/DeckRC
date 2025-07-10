    //
// Created by miantao on 25-6-25.
//

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H
#include "behavior.h"

#include <vector>
#include <SDL.h>
#include <string>

enum class ChannelBoundType {
    clamp, free, modulo, loop //, bounce
};

class Inputs {
public:
    Inputs(int n_channels);

    ~Inputs();

    bool cycle();

    bool cycle(std::vector<ChannelDataType> &channel_buffer);

    std::vector<ChannelDataType> getChannels() const;

    void getChannels(std::vector<ChannelDataType> &channel_buffer) const;

    std::vector<ChannelBoundType> channel_bounds;

    // Functions for JSON serialization
    bool saveToJson(const std::string& filename) const;
    bool loadFromJson(const std::string& filename);

protected:
    std::vector<int> gamepad_indices;
    std::vector<SDL_GameController*> gamepads;

    std::vector<ChannelDataType> channels_raw;
    std::vector<ChannelDataType> channel_biases;  // Per-channel biases
    std::vector<ChannelDataType> channel_limits;  // Per-channel limits

    std::vector<InputBehavior> cycle_behaviors;
    std::vector<KeyBehavior> key_down_behaviors;
    std::vector<KeyBehavior> key_up_behaviors;
    std::vector<ButtonBehavior> button_up_behaviors;
    std::vector<ButtonBehavior> button_down_behaviors;
    std::vector<AxisBehavior> axis_behaviors;

    bool processEvents();

    void keyDown(const SDL_Keycode &key);

    void keyUp(const SDL_Keycode &key);

    void controllerButtonDown(const Uint8 &button, const SDL_JoystickID &which);

    void controllerButtonUp(const Uint8 &button, const SDL_JoystickID &which);

    void controllerAxisMotion(const Uint8 &axis, const Sint16 &value, const SDL_JoystickID &which);

public:
    void clear() {
        cycle_behaviors.clear();
        key_down_behaviors.clear();
        key_up_behaviors.clear();
        button_up_behaviors.clear();
        button_down_behaviors.clear();
        axis_behaviors.clear();
    }

    void clear(int channel_index) {
        std::erase_if(cycle_behaviors, [channel_index](const InputBehavior &input_behavior) {return input_behavior.channel_index == channel_index;});
        std::erase_if(key_down_behaviors, [channel_index](const InputBehavior &input_behavior) {return input_behavior.channel_index == channel_index;});
        std::erase_if(key_up_behaviors, [channel_index](const InputBehavior &input_behavior) {return input_behavior.channel_index == channel_index;});
        std::erase_if(button_down_behaviors, [channel_index](const InputBehavior &input_behavior) {return input_behavior.channel_index == channel_index;});
        std::erase_if(button_up_behaviors, [channel_index](const InputBehavior &input_behavior) {return input_behavior.channel_index == channel_index;});
        std::erase_if(axis_behaviors, [channel_index](const InputBehavior &input_behavior) {return input_behavior.channel_index == channel_index;});
    }

    void add(int channel_index, const SDL_KeyCode &key, double value, InputMode mode=InputMode::set, bool on_release=false) {
        if (key==SDLK_UNKNOWN) {
            cycle_behaviors.emplace_back(channel_index, value, mode);
        } else if (on_release) {
            key_up_behaviors.emplace_back(channel_index, value, key, mode);
        } else {
            key_down_behaviors.emplace_back(channel_index, value, key, mode);
        }
    }

    void addTap(int channel_index, const SDL_KeyCode &key, double value) {
        key_down_behaviors.emplace_back(channel_index, value, key);
        cycle_behaviors.emplace_back(channel_index, 0);
    }

    void addRelease(int channel_index, const SDL_KeyCode &key, double value) {
        key_up_behaviors.emplace_back(channel_index, value, key);
        cycle_behaviors.emplace_back(channel_index, 0);
    }

    void addHold(int channel_index, const SDL_KeyCode &key, double value) {
        key_down_behaviors.emplace_back(channel_index, value, key);
        key_up_behaviors.emplace_back(channel_index, 0, key);
    }

    void addIncrement(int channel_index, const SDL_KeyCode &key, double value) {
        key_down_behaviors.emplace_back(channel_index, value, key, InputMode::increment);
    }

    void addToggle(int channel_index, const SDL_KeyCode &key, double value) {
        key_down_behaviors.emplace_back(channel_index, value, key, InputMode::toggle);
    }

    void addToggleSymmetric(int channel_index, const SDL_KeyCode &key, double value) {
        key_down_behaviors.emplace_back(channel_index, value, key, InputMode::toggle_symmetric);
        channels_raw.at(channel_index) = value;
    }

    void addTap(int channel_index, const Uint8 &button, const SDL_JoystickID &which, double value) {
        button_down_behaviors.emplace_back(channel_index, value, button, which);
        cycle_behaviors.emplace_back(channel_index, 0);
    }

    void addRelease(int channel_index, const Uint8 &button, const SDL_JoystickID &which, double value) {
        button_up_behaviors.emplace_back(channel_index, value, button, which);
        cycle_behaviors.emplace_back(channel_index, 0);
    }

    void addHold(int channel_index, const Uint8 &button, const SDL_JoystickID &which, double value) {
        button_down_behaviors.emplace_back(channel_index, value, button, which);
        button_up_behaviors.emplace_back(channel_index, 0, button, which);
    }

    void addIncrement(int channel_index, const Uint8 &button, const SDL_JoystickID &which, double value) {
        button_down_behaviors.emplace_back(channel_index, value, button, which, InputMode::increment);
    }

    void addToggle(int channel_index, const Uint8 &button, const SDL_JoystickID &which, double value) {
        button_down_behaviors.emplace_back(channel_index, value, button, which, InputMode::toggle);
    }

    void addToggleSymmetric(int channel_index, const Uint8 &button, const SDL_JoystickID &which, double value) {
        button_down_behaviors.emplace_back(channel_index, value, button, which, InputMode::toggle_symmetric);
        channels_raw.at(channel_index) = value;
    }

    void addAxis(int channel_index, const Uint8 &axis, const SDL_JoystickID &which, double value, AxisAsButton as_button=AxisAsButton::no, double threshold = 0, InputMode mode=InputMode::set) {
        axis_behaviors.emplace_back(channel_index, value, axis, which, as_button, threshold, mode);
    }

    void addAxisTap(int channel_index, const Uint8 &axis, const SDL_JoystickID &which, double value, double threshold = 0) {
        axis_behaviors.emplace_back(channel_index, value, axis, which, AxisAsButton::down, threshold, InputMode::set);
        cycle_behaviors.emplace_back(channel_index, 0);
    }

    void addAxisHold(int channel_index, const Uint8 &axis, const SDL_JoystickID &which, double value, double threshold = 0) {
        axis_behaviors.emplace_back(channel_index, value, axis, which, AxisAsButton::down, threshold, InputMode::set);
        axis_behaviors.emplace_back(channel_index, 0, axis, which, AxisAsButton::up, threshold, InputMode::set);
    }

    void addAxisRelease(int channel_index, const Uint8 &axis, const SDL_JoystickID &which, double value, double threshold = 0) {
        axis_behaviors.emplace_back(channel_index, value, axis, which, AxisAsButton::up, threshold, InputMode::set);
        axis_behaviors.emplace_back(channel_index, 0, axis, which, AxisAsButton::down, threshold, InputMode::set);
    }

    void addAxisIncrement(int channel_index, const Uint8 &axis, const SDL_JoystickID &which, double value, double threshold = 0) {
        axis_behaviors.emplace_back(channel_index, value, axis, which, AxisAsButton::down, threshold, InputMode::increment);
    }

    void addAxisToggle(int channel_index, const Uint8 &axis, const SDL_JoystickID &which, double value, double threshold = 0) {
        axis_behaviors.emplace_back(channel_index, value, axis, which, AxisAsButton::down, threshold, InputMode::toggle);
    }

    void addAxisToggleSymmetric(int channel_index, const Uint8 &axis, const SDL_JoystickID &which, double value, double threshold = 0) {
        axis_behaviors.emplace_back(channel_index, value, axis, which, AxisAsButton::down, threshold, InputMode::toggle_symmetric);
        channels_raw.at(channel_index) = value;
    }

};

#endif //INPUTCONTROLLER_H