#include <SDL.h>
#include <variant>

enum class InputType {
    None,
    Keyboard,
    JoystickButton,
    JoystickAxis
};

struct JoystickButton {
    Uint8 button;
    SDL_JoystickID joystick_id;
};

struct JoystickAxis {
    Uint8 axis;
    SDL_JoystickID joystick_id;
};

enum class ChannelModes {
    KEYBOARD_KEY_HOLD,
    KEYBOARD_KEY_RELEASE,
    KEYBOARD_KEY_INCREMENT,
    KEYBOARD_KEY_TOGGLE,
    KEYBOARD_KEY_TOGGLE_SYMETRIC,
    KEYBOARD_KEY_TAP,

    JOYSTICK_BUTTON_HOLD,
    JOYSTICK_BUTTON_RELEASE,
    JOYSTICK_BUTTON_INCREMENT,
    JOYSTICK_BUTTON_TOGGLE,
    JOYSTICK_BUTTON_TOGGLE_SYMETRIC,
    JOYSTICK_BUTTON_TAP,

    JOYSTICK_AXIS,
    JOYSTICK_AXIS_HOLD,
    JOYSTICK_AXIS_TOGGLE,
    JOYSTICK_AXIS_TOGGLE_SYMETRIC,
    JOYSTICK_AXIS_INCREMENT,

    NONE
};

struct InputConfig {
    int channel = -1;
    InputType type = InputType::None;
    SDL_Event raw_event;

    using InputVariant = std::variant<std::monostate, SDL_Keycode, JoystickButton, JoystickAxis>;

    InputVariant input_data;

    ChannelModes mode = ChannelModes::NONE;
};
