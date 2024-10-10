#define SCREEN_W 800
#define SCREEN_H 600

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

bool keys[256];

SDL_Window* window;
SDL_Renderer* renderer;
SDL_GLContext gl_context;

GLuint textureID;
GLuint fontID;

SDL_Color c = { 255, 255, 255, 255 };

int s_x = 100;
int s_y = 100;
int s_w = 200;
int s_h = 200;

class PLAYER {
public:
    float x, y, w, h;
    int move_speed = 10;
    float theta;
    SDL_Color c;

    PLAYER(int _x, int _y, int _w, int _h, SDL_Color _c) {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        c = _c;
    }
};

PLAYER player(50, 50, 200, 200, c);

std::vector<OBJECT::DEFAULT> objects;

void add_objects();

void handle_keydown(SDL_Event e);
void handle_keyup(SDL_Event e);

void move_player();

void update_gravity();

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
            if (event.type == SDL_KEYDOWN)  handle_keydown(event);
            if (event.type == SDL_KEYUP)    handle_keyup(event);
        }

        move_player();
        update_gravity();

        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //GAME_WINDOW::draw_triangle(sizeof(vertices));

        fontID = GAME_WINDOW::load_font("assets/fonts/SPACE.ttf", 24, "Hello", c);
        GAME_WINDOW::draw_font(fontID, 50, 50, 400, 100);
        textureID = GAME_WINDOW::load_texture("assets/face.png");
        GAME_WINDOW::draw_image(textureID, player.x, player.y, player.w, player.h, player.theta);

        for (int i = 0; i < objects.size(); i++) {
            objects[i].textureID = GAME_WINDOW::load_texture("assets/face.png");
            GAME_WINDOW::draw_image(objects[i].textureID, objects[i].x, objects[i].y, objects[i].w, objects[i].h, objects[i].theta);
            
            glDeleteTextures(1, &objects[i].textureID);
        }

        glDeleteTextures(1, &fontID);
        glDeleteTextures(1, &textureID);

        SDL_GL_SwapWindow(window);
        
        end_timer();
    }

}

int main() {
    window = GAME_WINDOW::create_window(SCREEN_W, SCREEN_H, "SDL2 With OpenGL");
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
    glOrtho(0, SCREEN_W, SCREEN_H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    //SDL_Color c = { 255, 255, 255, 255 };

    textureID = GAME_WINDOW::load_texture("assets/face.png");
    //fontID    = GAME_WINDOW::load_font("assets/fonts/SPACE.ttf", 24, "Vai a merda", c);

    //GAME_WINDOW::config_triangle_shader(vertices, sizeof(vertices));
    //GAME_WINDOW::config_square_shader(square_vertices, sizeof(square_vertices));
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    add_objects();

    SDL_Color c = { 255, 255, 255, 255 };
    GAME_WINDOW::load_font("assets/fonts/SPACE.ttf", 24, "Hello", c);
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
    OBJECT::DEFAULT new_obj(BODY_TYPE::DYNAMIC, 500, 50, 100, 100, 0, 9.8, "assets/face.png");

    objects.push_back(new_obj);
}

void handle_keydown(SDL_Event e) {
    if (e.key.keysym.sym > 256) return;
    std::cout << "key: " << e.key.keysym.sym << "\n";
    keys[e.key.keysym.sym] = true;
}

void handle_keyup(SDL_Event e) {
    if (e.key.keysym.sym > 256) return;
    keys[e.key.keysym.sym] = false;
}

void move_player() {
    player.theta += 0.01;
    if (player.theta < -M_PI * 2) player.theta += M_PI * 2;
    if (player.theta >  M_PI * 2) player.theta -= M_PI * 2;

    if (keys[SDLK_a]) player.x -= player.move_speed;
    if (keys[SDLK_d]) player.x += player.move_speed;
    if (keys[SDLK_w]) player.y -= player.move_speed;
    if (keys[SDLK_s]) player.y += player.move_speed;
}

void update_gravity() {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i].type != BODY_TYPE::DYNAMIC) break;

        objects[i].y += objects[i].gravity;
    }
}