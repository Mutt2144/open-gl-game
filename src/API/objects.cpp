#include <iostream>

#include "objects.hpp"

OBJECT::DEFAULT::DEFAULT(int _type, float _x, float _y, float _w, float _h, float _theta, float _gravity, const char* img_path) {
    type  = _type;
    x     = _x;     y = _y;
    w     = _w;     h = _h;
    theta = _theta;
    gravity = _gravity;

    textureID = GAME_WINDOW::load_texture(img_path);
}