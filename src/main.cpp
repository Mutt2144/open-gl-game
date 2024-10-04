#include <iostream>

#include "include/window.hpp"

#include "include_scripts.cpp"

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture* img_1;

void loop() {
    bool running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            SDL_RenderClear(renderer);

            SDL_Rect rect;
            rect.x = 0;
            rect.y = 0;
            rect.w = 100;
            rect.h = 100;

            SDL_RenderCopy(renderer, img_1, NULL, &rect);

            SDL_RenderPresent(renderer);
        }
    }
}

int main() {
    window = GAME_WINDOW::create_window(800, 600, "SDL Game");
    if (window == NULL) return -1;

    renderer = GAME_WINDOW::create_renderer(window);
    if (renderer == NULL) return -1;

    img_1 = GAME_WINDOW::load_texture("assets/face.bmp", renderer);

    loop();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}