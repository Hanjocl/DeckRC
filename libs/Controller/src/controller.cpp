//
// Created by miantao on 25-6-25.
//

#include "controller.h"

#include <algorithm>
#include <SDL_events.h>

Inputs::Inputs(int n_channels) : channels_raw(n_channels, 0), channel_bounds(n_channels, ChannelBoundType::clamp) {
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            gamepad_indices.push_back(i);
            gamepads.push_back(SDL_GameControllerOpen(i));
        }
    }
}

Inputs::~Inputs() {
    for (SDL_GameController *pGamepad : gamepads) {
        SDL_GameControllerClose(pGamepad);
    }
}

bool Inputs::cycle() {
    for (const InputBehavior &cycle_behavior : cycle_behaviors) {
        cycle_behavior(channels_raw);
    }

    bool is_running = processEvents();

    std::transform(channels_raw.cbegin(), channels_raw.cend(), channel_bounds.cbegin(), channels_raw.begin(), [this](const ChannelDataType &raw, const ChannelBoundType &bound) {
        switch (bound) {
            case ChannelBoundType::free:
                return raw;
            case ChannelBoundType::clamp:
                return std::clamp(raw, -channel_limit, channel_limit);
            case ChannelBoundType::modulo:
                return raw % channel_limit;
            case ChannelBoundType::loop:
                return std::div(raw + channel_limit, channel_limit*2).rem - channel_limit;
            default:
                return std::clamp(raw, -channel_limit, channel_limit);
        }
    });

    return is_running;
}

bool Inputs::cycle(std::vector<ChannelDataType> &channel_buffer) {

    bool is_running = cycle();

    getChannels(channel_buffer);

    return is_running;
}

std::vector<ChannelDataType> Inputs::getChannels() const {
    std::vector<ChannelDataType> channel_buffer(channels_raw.size());
    getChannels(channel_buffer);
    return channel_buffer;
}

void Inputs::getChannels(std::vector<ChannelDataType> &channel_buffer) const {
    std::transform(channels_raw.cbegin(), channels_raw.cend(), channel_buffer.begin(), [this](const ChannelDataType &raw) {
        return raw + channel_bias;
    });
}

bool Inputs::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
                keyDown(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                keyUp(event.key.keysym.sym);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                controllerButtonDown(event.cbutton.button, event.cbutton.which);
                break;
            case SDL_CONTROLLERBUTTONUP:
                controllerButtonUp(event.cbutton.button, event.cbutton.which);
                break;
            case SDL_CONTROLLERAXISMOTION:
                controllerAxisMotion(event.caxis.axis, event.caxis.value, event.caxis.which);
                break;
            default:
                break;
        }
    }

    return true;
}

void Inputs::keyDown(const SDL_Keycode &key) {
    for (const KeyBehavior &key_behavior : key_down_behaviors) {
        if (key == key_behavior.key) {
            key_behavior(channels_raw);
        }
    }
}

void Inputs::keyUp(const SDL_Keycode &key) {
    for (const KeyBehavior &key_behavior : key_up_behaviors) {
        if (key == key_behavior.key) {
            key_behavior(channels_raw);
        }
    }
}

void Inputs::controllerButtonDown(const Uint8 &button, const SDL_JoystickID &which) {
    for (const ButtonBehavior &button_behavior : button_down_behaviors) {
        if (button == button_behavior.button and which == button_behavior.which) {
            button_behavior(channels_raw);
        }
    }
}

void Inputs::controllerButtonUp(const Uint8 &button, const SDL_JoystickID &which) {
    for (const ButtonBehavior &button_behavior : button_up_behaviors) {
        if (button == button_behavior.button and which == button_behavior.which) {
            button_behavior(channels_raw);
        }
    }
}

void Inputs::controllerAxisMotion(const Uint8 &axis, const Sint16 &value, const SDL_JoystickID &which) {
    for (AxisBehavior &axis_behavior : axis_behaviors) {
        if (axis == axis_behavior.button and which == axis_behavior.which) {
            axis_behavior(channels_raw, value);
        }
    }
}
