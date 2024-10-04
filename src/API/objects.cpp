#include <iostream>

#include "objects.hpp"

OBJECT::SIMPLE_IMAGE::SIMPLE_IMAGE(vec2 _pos, vec2 _siz, const char* path, SDL_Renderer* renderer) {
    rect.x = _pos.x;
    rect.y = _pos.y;
    rect.w = _siz.x;
    rect.h = _siz.y;

    img = GAME_WINDOW::load_texture(path, renderer);
}

OBJECT::SIMPLE_IMAGE::~SIMPLE_IMAGE() {
    std::cout << "Deleted\n";
}