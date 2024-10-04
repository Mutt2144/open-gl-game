#include <iostream>
#include <vector>

#include "include/window.hpp"
#include "API/objects.hpp"

#include "include_scripts.cpp"

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture* img_1;

std::vector<OBJECT::SIMPLE_IMAGE> simple_images;

void add_objects();

void loop() {
    bool running = 1;
    SDL_Event event;

    while (running) {
        // issue: doesn't update correctly
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            simple_images[0].rect.x += 1;

            SDL_RenderClear(renderer);

            for (int i = 0; i < simple_images.size(); i++) {
                OBJECT::SIMPLE_IMAGE* obj = &simple_images[i];

                SDL_RenderCopy(renderer, obj->img, NULL, &obj->rect);
            }

            SDL_RenderPresent(renderer);
        }
    }
}

int main() {
    window = GAME_WINDOW::create_window(800, 600, "SDL Game");
    if (window == NULL) return -1;

    renderer = GAME_WINDOW::create_renderer(window);
    if (renderer == NULL) return -1;

    //img_1 = GAME_WINDOW::load_texture("assets/face.bmp", renderer);

    add_objects();

    loop();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}


void add_objects() {
    OBJECT::SIMPLE_IMAGE img({ 0, 0 }, { 100, 100 }, "assets/face.bmp", renderer);

    simple_images.push_back(img);
}