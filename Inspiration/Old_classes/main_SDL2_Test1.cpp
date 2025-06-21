#define SDL_MAIN_HANDLED

#include <iostream>
#include <unistd.h>
#include <deque>
#include <SDL.h>

int main(int argc, char** args) {
    // init
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw SDL_GetError();
    } else {
        std::cout << "SDL_Init success! \n";
    }

    SDL_Window *pWindow = SDL_CreateWindow(
        "my window by hanjo", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        800, 
        400, 
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

    // SDL_RendererInfo rendererInfo;
    // SDL_GetRendererInfo(renderer.get(), &rendererInfo);
    // textureFormatEnum = rendererInfo.texture_formats[0];

    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    
    int t = 0;
    bool is_running = true;

    bool a_is_held;

    std::deque<int> controller_indices;
    std::deque<SDL_GameController*> controller_ptrs;
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            controller_indices.push_back(i);
            controller_ptrs.push_back(SDL_GameControllerOpen(i));
            std::cout << "controller " << i << " opened \n";
        }
    }

    while (is_running) {
        t++;
        int x = t/100 % 100;

        // handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "quit event received \n";
                is_running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    std::cout << "escape key pressed \n";
                    is_running = false;
                } else if (event.key.keysym.sym == SDLK_a) {
                    std::cout << "a key pressed \n";
                    a_is_held = true;
                }
            }

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_a) {
                    std::cout << "a key released \n";
                    a_is_held = false;
                }
            }

            if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                std::cout << "controller button pressed: " << int(event.cbutton.button) << "\n";
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                    std::cout << "controller A button pressed \n";
                    a_is_held = true;
                } else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X) {
                }
            }

            if (event.type == SDL_CONTROLLERBUTTONUP) {
                std::cout << "controller button released: " << int(event.cbutton.button) << "\n";
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                    std::cout << "controller A button released \n";
                    a_is_held = false;
                }
            }

            if (event.type == SDL_CONTROLLERDEVICEADDED) {
                std::cout << "controller added: " << event.cdevice.which << "\n";
                controller_indices.push_back(event.cdevice.which);
                controller_ptrs.push_back(SDL_GameControllerOpen(event.cdevice.which));
            }
            if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
                std::cout << "controller removed: " << event.cdevice.which << "\n";
                for (size_t i = 0; i < controller_indices.size(); i++) {
                    if (controller_indices[i] == event.cdevice.which) {
                        SDL_GameControllerClose(controller_ptrs[i]);
                        controller_indices.erase(controller_indices.begin() + i);
                        controller_ptrs.erase(controller_ptrs.begin() + i);
                        break;
                    }
                }
            }
            if (event.type == SDL_CONTROLLERAXISMOTION) {
                std::cout << "controller axis motion: " << int(event.caxis.axis) << " value: " << event.caxis.value << "\n";
                if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
                    // handle left stick x-axis motion
                } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                    // handle left stick y-axis motion
                }
            }
        }

        if (a_is_held) {
            SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
        } else {
            SDL_SetRenderDrawColor(pRenderer, 0,0,0, 255);
        }
        SDL_RenderClear(pRenderer);

        
        SDL_SetRenderDrawColor(pRenderer, 255,0,0, 255);
        SDL_Rect rect = {x, 100, 200, 200 };
        SDL_RenderFillRect(pRenderer, &rect); // draw a red rectangle

        // draw a dog

        SDL_RenderPresent(pRenderer); //pg.display.flipState : paint what's on the renderer onto its window
    }

    // quit
    SDL_Quit();
    std::cout << "exited SDL \n";

    return 0;
}