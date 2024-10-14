#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include "../API/objects.hpp"

/*struct VEC2 {
    float x, y;
};
*/

namespace GAME_WINDOW {
    unsigned int VBO, VAO, EBO;
    unsigned int shaderProgram;

    void config_opengl();
    SDL_Window* create_window(int w, int h, const char* title);
    SDL_Renderer* create_renderer(SDL_Window* window);

    GLuint load_font(const char* path, int size, const char* message, SDL_Color color);
    void draw_font(GLuint textureID, int x, int y, int w, int h);

    SDL_GLContext create_open_gl_renderer(SDL_Window* window);
    bool load_gl_function(SDL_Window* window, SDL_GLContext gl_context);

    const char* load_shader(const char* shaderPath);
    unsigned int compile_shader(unsigned int type, const char* source);
    unsigned int create_shader_program(const char* vertex_shader_source, const char* fragment_shader_source);

    GLuint load_texture(const char* path);

    void config_square_shader(float vertices[], int num_vertices);
    void config_triangle_shader(float vertices[], int num_vertices);
    void draw_triangle(int num_vertices);

    void config_image_shader(float vertices[], int num_vertices, unsigned index[], int num_index);
    void draw_image(GLuint textureID, float x, float y, float w, float h, float theta);

    VEC2 apply_rotation(VEC2 point, VEC2 size, float theta);
}

#endif