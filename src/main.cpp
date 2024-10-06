#include <iostream>
#include <vector>

#include "glad.c"

#include "include/window.hpp"
#include "API/objects.hpp"

#include "fps-controller.cpp"
#include "include_scripts.cpp"

float vertices[] = {
    0.0f,  0.5f,    // top
   -0.5f, -0.5f,    // left
    0.5f, -0.5f     // right
};

float square_vertices[] = {
    // first triangle
    0.5f,  0.5f,    // top-right
    0.5f, -0.5f,    // bottom-right
   -0.5f, -0.5f,    // bottom-left

   // second triangle
   -0.5f, -0.5f,    // bottom-left
   -0.5f,  0.5f,    // top-left
    0.5f,  0.5f     // top-right
};


SDL_Window* window;
SDL_Renderer* renderer;
SDL_GLContext gl_context;

GLuint textureID;

//SDL_Texture* img_1;

//std::vector<OBJECT::SIMPLE_IMAGE> simple_images;

void add_objects();

void loop() {
    bool running = 1;
    SDL_Event event;

    while (running) {
        start_timer();

        // event manager
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //GAME_WINDOW::draw_triangle(sizeof(vertices));

        glBindTexture(GL_TEXTURE_2D, textureID);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);   glVertex2f(100, 100);   // top-left
        glTexCoord2f(1.0f, 0.0f);   glVertex2f(300, 100);   // top-right
        glTexCoord2f(1.0f, 1.0f);   glVertex2f(300, 300);   // bottom-direito
        glTexCoord2f(0.0f, 1.0f);   glVertex2f(100, 300);   // bottom-esquerdo
        glEnd();


        SDL_GL_SwapWindow(window);
    }
}

int main() {
    window = GAME_WINDOW::create_window(800, 600, "SDL2 With OpenGL");
    if (window == NULL) return -1;

    gl_context = GAME_WINDOW::create_open_gl_renderer(window);
    if (gl_context == NULL) return -1;

    if (!GAME_WINDOW::load_gl_function(window, gl_context)) return -1;

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cerr << "Cannot define V-Sync: " << SDL_GetError() << "\n";
        return -1;
    }


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    textureID = GAME_WINDOW::load_texture("assets/face.png");

    //GAME_WINDOW::config_triangle_shader(vertices, sizeof(vertices));
    //GAME_WINDOW::config_square_shader(square_vertices, sizeof(square_vertices));
    glEnable(GL_TEXTURE_2D);   
    set_fps(60);
    loop();

    glDeleteTextures(1, &textureID);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();

    return 0;
}


void add_objects() {
    //OBJECT::SIMPLE_IMAGE img({ 0, 0 }, { 100, 100 }, "assets/face.bmp", renderer);

    //simple_images.push_back(img);
}