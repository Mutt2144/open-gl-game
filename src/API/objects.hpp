#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct VEC2 {
    float x, y;
};

namespace BODY_TYPE {
    int STATIC = 0;
    int DYNAMIC = 1;
};

namespace OBJECT {
    class DEFAULT {
    public:
        int type;
        float x, y, w, h;
        float theta;
        float gravity;
        GLuint textureID;

        VEC2 speed;

        DEFAULT(int _type, float _x, float _y, float _w, float _h, float theta, float _gravity, const char* img_path);
    };
}

#endif