#include <iostream>

#include "window.hpp"

SDL_Window* GAME_WINDOW::create_window(int w, int h, const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Cannot initialize SDL: " << SDL_GetError() << "\n";
        return NULL;
    }

    SDL_Window* win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!win) {
        std::cerr << "Cannot create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return NULL;
    }

    return win;
}

SDL_Renderer* GAME_WINDOW::create_renderer(SDL_Window* window) {
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!rend) {
        std::cerr << "Cannot create renderer: " << SDL_GetError() << "\n";
        SDL_Quit();
        return NULL;
    }

    return rend;
}


SDL_Texture* GAME_WINDOW::load_texture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* temp = SDL_LoadBMP(path);
    if (temp == nullptr) {
        std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << "\n";
        return NULL;
    }

    // upload surface to renderer, and then, free the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    if (texture == nullptr) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << "\n";
        return NULL;
    }

    return texture;
}