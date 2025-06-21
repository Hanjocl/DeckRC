#include "inputs.h"

//class Inputs
Keys Inputs::sdl_to_keys(const SDL_Keycode& keyCode) {
    Keys output;
    switch (keyCode) {
        case SDLK_a :
            output = Keys::a;
            break;
        case SDLK_b :
            output = Keys::b;
            break;
        case SDLK_c :
            output = Keys::c;
            break;
        case SDLK_d :
            output = Keys::d;
            break;
        case SDLK_e :
            output = Keys::e;
            break;
        case SDLK_f :
            output = Keys::f;
            break;
        case SDLK_g :
            output = Keys::g;
            break;
        case SDLK_h :
            output = Keys::h;
            break;
        case SDLK_i :
            output = Keys::i;
            break;
        case SDLK_j :
            output = Keys::j;
            break;
        case SDLK_k :
            output = Keys::k;
            break;
        case SDLK_l :
            output = Keys::l;
            break;
        case SDLK_m :
            output = Keys::m;
            break;
        case SDLK_n :
            output = Keys::n;
            break;
        case SDLK_o :
            output = Keys::o;
            break;
        case SDLK_p :
            output = Keys::p;
            break;
        case SDLK_q :
            output = Keys::q;
            break;
        case SDLK_r :
            output = Keys::r;
            break;
        case SDLK_s :
            output = Keys::s;
            break;
        case SDLK_t :
            output = Keys::t;
            break;
        case SDLK_u :
            output = Keys::u;
            break;
        case SDLK_v :
            output = Keys::v;
            break;
        case SDLK_w :
            output = Keys::w;
            break;
        case SDLK_x :
            output = Keys::x;
            break;
        case SDLK_y :
            output = Keys::y;
            break;
        case SDLK_z :
            output = Keys::z;
            break;
        case SDLK_UP :
            output = Keys::up;
            break;
        case SDLK_DOWN :
            output = Keys::down;
            break;
        case SDLK_LEFT :
            output = Keys::left;
            break;
        case SDLK_RIGHT :
            output = Keys::right;
            break;
        case SDLK_SPACE :
            output = Keys::space;
            break;
        case SDLK_BACKSPACE :
            output = Keys::backspace;
            break;
        case SDLK_ESCAPE :
            output = Keys::escape;
            break;
        case SDLK_LSHIFT :
            output = Keys::l_shift;
            break;
        case SDLK_RSHIFT :
            output = Keys::r_shift;
            break;
        case SDLK_LCTRL:
            output = Keys::l_control;
            break;
        case SDLK_RCTRL:
            output = Keys::r_control;
            break;
        case SDLK_TAB:
            output = Keys::tab;
            break;
        default :
            output = Keys::other;
            break;
    }
    return output;
}

Inputs::Inputs(){
    std::unordered_set<Keys> all_keys({Keys::a, Keys::b, Keys::c, Keys::d, Keys::e, Keys::f, Keys::g, Keys::h, Keys::i, Keys::j, Keys::k, Keys::l, Keys::m, Keys::n, Keys::o, Keys::p, Keys::q, Keys::r, Keys::s, Keys::t, Keys::u, Keys::v, Keys::w, Keys::x, Keys::y, Keys::z, Keys::up, Keys::down, Keys::left, Keys::right, Keys::space, Keys::backspace, Keys::escape, Keys::l_shift, Keys::r_shift, Keys::l_control, Keys::r_control, Keys::tab, Keys::left_button, Keys::right_button, Keys::middle_button, Keys::none, Keys::other});

    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(pressedMap, pressedMap.end()), [](const Keys &key){return std::pair(key, false);});
    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(downMap, downMap.end()), [](const Keys &key){return std::pair(key, false);});
    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(upMap, upMap.end()), [](const Keys &key){return std::pair(key, false);});
    std::transform(all_keys.cbegin(), all_keys.cend(), std::inserter(downTimeMap, downTimeMap.end()), [](const Keys &key){return std::pair(key, 0.0);});

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            gamepads.emplace_back(i);
        }
    }
}

Inputs::~Inputs() {
    for (Gamepad &gamepad : this->gamepads) {
        gamepad.close();
    }
}

void Inputs::keyDown(const Keys& key, double time){
    //the keyboard recognises holding a inputKey as multiple downs without up. To filter these out, if a inputKey is already pressed, it can't go down a second time.
    if (not pressedMap[key]) {
        downMap[key] = true;
        pressedMap[key] = true;
        downTimeMap[key] = time;
    }
}

void Inputs::keyUp(const Keys& key){
    upMap[key] = true;
    pressedMap[key] = false;
}

void Inputs::mouseMotion(){
    SDL_GetMouseState(&mousePosOnScreen.x, &mousePosOnScreen.y);
}

void Inputs::update(double dt, Game& game){
    try {
        mousePosInGame = game.pGraphics->pixelToPoint(mousePosOnScreen);
    } catch (const std::invalid_argument &error) {
        //do not update the position then;
    }

    wasd_vec.x = pressedMap[Keys::d] - pressedMap[Keys::a];
    wasd_vec.y = pressedMap[Keys::w] - pressedMap[Keys::s];

    arrows_vec.x = pressedMap[Keys::right] - pressedMap[Keys::left];
    arrows_vec.y = pressedMap[Keys::up] - pressedMap[Keys::down];

}

void Inputs::reset() {
    for (auto & item : upMap)
        item.second = false;

    for (auto & item : downMap)
        item.second = false;

    for (Gamepad &gc : gamepads) {
        gc.reset();
    }
}

//void handleEvents()
void handleEvents(Inputs& inputs, double gameTime, bool& isRunning){
    inputs.reset();

    SDL_Event event;                //events
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                inputs.keyDown(Inputs::sdl_to_keys(event.key.keysym.sym), gameTime);
                break;
            case SDL_KEYUP:
                inputs.keyUp(Inputs::sdl_to_keys(event.key.keysym.sym));
                break;
            case SDL_MOUSEMOTION:
                inputs.mouseMotion();
                break;
            case SDL_MOUSEBUTTONDOWN:
                inputs.keyDown(std::vector({Keys::left_button, Keys::middle_button, Keys::right_button})[event.button.button - 1], gameTime);
                break;
            case SDL_MOUSEBUTTONUP:
                inputs.keyUp(std::vector({Keys::left_button, Keys::middle_button, Keys::right_button})[event.button.button - 1]);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                for (Gamepad &gc : inputs.gamepads) {
                    if (gc.joystick_index == event.cbutton.which){
                        gc.keyDown(Gamepad::sdl_to_keys(event.cbutton.button), gameTime);
                        break;
                    }
                }
                break;
            case SDL_CONTROLLERBUTTONUP:
                for (Gamepad &gc : inputs.gamepads) {
                    if (gc.joystick_index == event.cbutton.which){
                        gc.keyUp(Gamepad::sdl_to_keys(event.cbutton.button));
                        break;
                    }
                }
                break;
            case SDL_CONTROLLERAXISMOTION:
                for (Gamepad &gc : inputs.gamepads) {
                    if (gc.joystick_index == event.caxis.which){
                        gc.axisMotion(event.caxis.axis, event.caxis.value);
                        break;
                    }
                }
                break;
        }
    }
}