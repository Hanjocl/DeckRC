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