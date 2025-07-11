enum class InputType {
    None,
    Keyboard,
    JoystickButton,
    JoystickAxis
};

struct InputEvent {
    InputType type = InputType::None;
    SDL_Event raw_event;
    union {
        SDL_Keycode key;
        struct {
            Uint8 button;
            SDL_JoystickID joystick_id;
        } joystick_button;
        struct {
            Uint8 axis;
            Sint16 value;
            SDL_JoystickID joystick_id;
        } joystick_axis;
    };
};
