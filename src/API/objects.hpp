#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/window.hpp"

struct vec2 {
    float x, y;
};

namespace OBJECT {
    class SIMPLE_IMAGE {
    public:
        SDL_Rect rect;
        SDL_Texture* img;

        SIMPLE_IMAGE(vec2 _pos, vec2 _siz, const char* path, SDL_Renderer* renderer);

        ~SIMPLE_IMAGE();
    };
}

#endif