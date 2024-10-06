#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>

namespace GAME_WINDOW {
    unsigned int VBO, VAO, EBO;
    unsigned int shaderProgram;

    void config_opengl();
    SDL_Window* create_window(int w, int h, const char* title);
    SDL_Renderer* create_renderer(SDL_Window* window);

    SDL_GLContext create_open_gl_renderer(SDL_Window* window);
    bool load_gl_function(SDL_Window* window, SDL_GLContext gl_context);

    unsigned int compile_shader(unsigned int type, const char* source);
    unsigned int create_shader_program(const char* vertex_shader_source, const char* fragment_shader_source);

    GLuint load_texture(const char* path);

    void config_square_shader(float vertices[], int num_vertices);
    void config_triangle_shader(float vertices[], int num_vertices);
    void draw_triangle(int num_vertices);

    void config_image_shader(float vertices[], int num_vertices, unsigned index[], int num_index);
    void draw_image(int num_vertices, unsigned int texture);
}

#endif