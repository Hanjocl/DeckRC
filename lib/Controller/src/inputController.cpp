//
// Created by miantao on 25-6-25.
//

#include "inputController.h"
#include <algorithm>
#include <SDL_events.h>
#include <fstream>
#include <../nlohmann/json.hpp>

using json = nlohmann::json;

Inputs::Inputs(int n_channels) : channels_raw(n_channels, 0), channel_bounds(n_channels, ChannelBoundType::clamp), channel_biases(n_channels, 1500), channel_limits(n_channels, 500) {
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
        int channel_index = &raw - &channels_raw[0];
        switch (bound) {
            case ChannelBoundType::free:
                return raw;
            case ChannelBoundType::clamp:
                return std::clamp(raw, -channel_limits[channel_index], channel_limits[channel_index]);
            case ChannelBoundType::modulo:
                return raw % channel_limits[channel_index];
            case ChannelBoundType::loop:
                return std::div(raw + channel_limits[channel_index], channel_limits[channel_index]*2).rem - channel_limits[channel_index];
            default:
                return std::clamp(raw, -channel_limits[channel_index], channel_limits[channel_index]);
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
    std::transform(channels_raw.cbegin(), channels_raw.cend(), channel_biases.cbegin(), channel_buffer.begin(), [](const ChannelDataType &raw, const ChannelDataType &bias) {
        return raw + bias;
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
        if (button == button_behavior.button && which == button_behavior.which) {
            button_behavior(channels_raw);
        }
    }
}

void Inputs::controllerButtonUp(const Uint8 &button, const SDL_JoystickID &which) {
    for (const ButtonBehavior &button_behavior : button_up_behaviors) {
        if (button == button_behavior.button && which == button_behavior.which) {
            button_behavior(channels_raw);
        }
    }
}

void Inputs::controllerAxisMotion(const Uint8 &axis, const Sint16 &value, const SDL_JoystickID &which) {
    for (AxisBehavior &axis_behavior : axis_behaviors) {
        if (axis == axis_behavior.button && which == axis_behavior.which) {
            axis_behavior(channels_raw, value);
        }
    }
}

bool Inputs::saveToJson(const std::string& filename) const {
    json j;

    // Save channel configurations
    json channels_json = json::array();
    for (size_t channel_index = 0; channel_index < channels_raw.size(); ++channel_index) {
        json channel;
        channel["index"] = channel_index;
        channel["bias"] = channel_biases[channel_index];
        channel["limit"] = channel_limits[channel_index];

        // Save channel bound
        switch (channel_bounds[channel_index]) {
            case ChannelBoundType::clamp: channel["bound"] = "clamp"; break;
            case ChannelBoundType::free: channel["bound"] = "free"; break;
            case ChannelBoundType::modulo: channel["bound"] = "modulo"; break;
            case ChannelBoundType::loop: channel["bound"] = "loop"; break;
        }

        // Save cycle behaviors for this channel
        json cycle_behaviors_json = json::array();
        for (const auto& behavior : cycle_behaviors) {
            if (behavior.channel_index == static_cast<int>(channel_index)) {
                json b;
                b["value"] = behavior.getValue();
                b["mode"] = static_cast<int>(behavior.getMode());
                cycle_behaviors_json.push_back(b);
            }
        }
        channel["cycle_behaviors"] = cycle_behaviors_json;

        // Save key down behaviors for this channel
        json key_down_behaviors_json = json::array();
        for (const auto& behavior : key_down_behaviors) {
            if (behavior.channel_index == static_cast<int>(channel_index)) {
                json b;
                b["value"] = behavior.getValue();
                b["key"] = behavior.key;
                b["mode"] = static_cast<int>(behavior.getMode());
                key_down_behaviors_json.push_back(b);
            }
        }
        channel["key_down_behaviors"] = key_down_behaviors_json;

        // Save key up behaviors for this channel
        json key_up_behaviors_json = json::array();
        for (const auto& behavior : key_up_behaviors) {
            if (behavior.channel_index == static_cast<int>(channel_index)) {
                json b;
                b["value"] = behavior.getValue();
                b["key"] = behavior.key;
                b["mode"] = static_cast<int>(behavior.getMode());
                key_up_behaviors_json.push_back(b);
            }
        }
        channel["key_up_behaviors"] = key_up_behaviors_json;

        // Save button down behaviors for this channel
        json button_down_behaviors_json = json::array();
        for (const auto& behavior : button_down_behaviors) {
            if (behavior.channel_index == static_cast<int>(channel_index)) {
                json b;
                b["value"] = behavior.getValue();
                b["button"] = behavior.button;
                b["which"] = behavior.which;
                b["mode"] = static_cast<int>(behavior.getMode());
                button_down_behaviors_json.push_back(b);
            }
        }
        channel["button_down_behaviors"] = button_down_behaviors_json;

        // Save button up behaviors for this channel
        json button_up_behaviors_json = json::array();
        for (const auto& behavior : button_up_behaviors) {
            if (behavior.channel_index == static_cast<int>(channel_index)) {
                json b;
                b["value"] = behavior.getValue();
                b["button"] = behavior.button;
                b["which"] = behavior.which;
                b["mode"] = static_cast<int>(behavior.getMode());
                button_up_behaviors_json.push_back(b);
            }
        }
        channel["button_up_behaviors"] = button_up_behaviors_json;

        // Save axis behaviors for this channel
        json axis_behaviors_json = json::array();
        for (const auto& behavior : axis_behaviors) {
            if (behavior.channel_index == static_cast<int>(channel_index)) {
                json b;
                b["value"] = behavior.getValue();
                b["axis"] = behavior.button;
                b["which"] = behavior.which;
                b["as_button"] = static_cast<int>(behavior.as_button);
                b["threshold"] = behavior.threshold;
                b["mode"] = static_cast<int>(behavior.getMode());
                axis_behaviors_json.push_back(b);
            }
        }
        channel["axis_behaviors"] = axis_behaviors_json;

        channels_json.push_back(channel);
    }
    j["channels"] = channels_json;

    // Write to file
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << j.dump(4);
        file.close();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Inputs::loadFromJson(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        json j;
        file >> j;
        file.close();

        // Clear existing behaviors
        clear();

        // Load channels
        channels_raw.resize(j.at("channels").size());
        channel_bounds.resize(j.at("channels").size());
        channel_biases.resize(j.at("channels").size());
        channel_limits.resize(j.at("channels").size());
        for (const auto& channel : j.at("channels")) {
            int channel_index = channel.at("index").get<int>();

            // Load channel settings
            channel_biases[channel_index] = channel.at("bias").get<ChannelDataType>();
            channel_limits[channel_index] = channel.at("limit").get<ChannelDataType>();

            // Load channel bound
            std::string bound = channel.at("bound").get<std::string>();
            if (bound == "clamp") channel_bounds[channel_index] = ChannelBoundType::clamp;
            else if (bound == "free") channel_bounds[channel_index] = ChannelBoundType::free;
            else if (bound == "modulo") channel_bounds[channel_index] = ChannelBoundType::modulo;
            else if (bound == "loop") channel_bounds[channel_index] = ChannelBoundType::loop;

            // Load cycle behaviors
            for (const auto& b : channel.at("cycle_behaviors")) {
                cycle_behaviors.emplace_back(
                    channel_index,
                    b.at("value").get<double>(),
                    static_cast<InputMode>(b.at("mode").get<int>())
                );
            }

            // Load key down behaviors
            for (const auto& b : channel.at("key_down_behaviors")) {
                key_down_behaviors.emplace_back(
                    channel_index,
                    b.at("value").get<double>(),
                    b.at("key").get<SDL_Keycode>(),
                    static_cast<InputMode>(b.at("mode").get<int>())
                );
            }

            // Load key up behaviors
            for (const auto& b : channel.at("key_up_behaviors")) {
                key_up_behaviors.emplace_back(
                    channel_index,
                    b.at("value").get<double>(),
                    b.at("key").get<SDL_Keycode>(),
                    static_cast<InputMode>(b.at("mode").get<int>())
                );
            }

            // Load button down behaviors
            for (const auto& b : channel.at("button_down_behaviors")) {
                button_down_behaviors.emplace_back(
                    channel_index,
                    b.at("value").get<double>(),
                    b.at("button").get<Uint8>(),
                    b.at("which").get<Uint16>(),
                    static_cast<InputMode>(b.at("mode").get<int>())
                );
            }

            // Load button up behaviors
            for (const auto& b : channel.at("button_up_behaviors")) {
                button_up_behaviors.emplace_back(
                    channel_index,
                    b.at("value").get<double>(),
                    b.at("button").get<Uint8>(),
                    b.at("which").get<Uint16>(),
                    static_cast<InputMode>(b.at("mode").get<int>())
                );
            }

            // Load axis behaviors
            for (const auto& b : channel.at("axis_behaviors")) {
                axis_behaviors.emplace_back(
                    channel_index,
                    b.at("value").get<double>(),
                    b.at("axis").get<Uint8>(),
                    b.at("which").get<Uint16>(),
                    static_cast<AxisAsButton>(b.at("as_button").get<int>()),
                    b.at("threshold").get<double>(),
                    static_cast<InputMode>(b.at("mode").get<int>())
                );
            }
        }

        return true;
    } catch (const std::exception& e) {
        return false;
    }
}