#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

namespace GAME_WINDOW {
    SDL_Window* create_window(int w, int h, const char* title);
    SDL_Renderer* create_renderer(SDL_Window* window);

    SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer);
}

#endif