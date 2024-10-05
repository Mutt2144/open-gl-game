#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

namespace GAME_WINDOW {
    unsigned int VBO, VAO;
    unsigned int shaderProgram;

    void config_opengl();
    SDL_Window* create_window(int w, int h, const char* title);
    SDL_Renderer* create_renderer(SDL_Window* window);

    SDL_GLContext create_open_gl_renderer(SDL_Window* window);
    bool load_gl_function(SDL_Window* window, SDL_GLContext gl_context);

    SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer);

    void config_triangle_shader(float vertices[], int num_vertices);
    void draw_triangle();
}

#endif