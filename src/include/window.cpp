#include <iostream>

#include "window.hpp"

void GAME_WINDOW::config_opengl() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

SDL_Window* GAME_WINDOW::create_window(int w, int h, const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Cannot initialize SDL: " << SDL_GetError() << "\n";
        return NULL;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cerr << "Cannot initialize IMG: " << IMG_GetError() << "\n";
        return NULL;
    }

    if (TTF_Init() < 0) {
        std::cerr << "cannot initialize TTF: " << TTF_GetError() << "\n";
        return NULL;
    }

    SDL_Window* win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
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

GLuint GAME_WINDOW::load_font(const char* path, int size, const char* message, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont(path, size);
    if (!font) {
        std::cerr << "Cannot load font: " << TTF_GetError() << "\n";
        return 0;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, color);
    if (!textSurface) {
        std::cerr << "Cannot create texture surface: " << TTF_GetError() << "\n";
        return 0;
    }


    GLuint textureID;
    glGenTextures(1, &textureID);

    SDL_Surface* textSurfaceConverted = SDL_ConvertSurfaceFormat(textSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurfaceConverted->w, textSurfaceConverted->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textSurfaceConverted->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(textSurfaceConverted);
    TTF_CloseFont(font);
    return textureID;
}

void GAME_WINDOW::draw_font(GLuint textureID, int x, int y, int w, int h) {
    //glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f);   glVertex2f(x,     y);
    glTexCoord2f(1.0f, 0.0f);   glVertex2f(x + w, y);
    glTexCoord2f(1.0f, 1.0f);   glVertex2f(x + w, y + h);
    glTexCoord2f(0.0f, 1.0f);   glVertex2f(x,     y + h);

    glEnd();
}



SDL_GLContext GAME_WINDOW::create_open_gl_renderer(SDL_Window* window) {
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "Cannot create OpenGL context: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    
    return gl_context;
}

bool GAME_WINDOW::load_gl_function(SDL_Window* window, SDL_GLContext gl_context) {
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Cannot load GL functions: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_GL_DeleteContext(gl_context);
        SDL_Quit();

        return 0;
    }

    return 1;
}

const char* GAME_WINDOW::load_shader(const char* shaderPath) {
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.open(shaderPath);

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    shaderCode = shaderStream.str();

    const char* shaderCodeCStr = shaderCode.c_str();
    return shaderCode.c_str();
}

unsigned int GAME_WINDOW::compile_shader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << "\n";
    }

    return shader;
}

unsigned int GAME_WINDOW::create_shader_program(const char* vertex_shader_source, const char* fragment_shader_source) {
    unsigned int vertexShader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    unsigned int fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking error: " << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


GLuint GAME_WINDOW::load_texture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);  // Carregar imagem usando SDL2_image
    if (!surface) {
        printf("Erro ao carregar a imagem: %s\n", SDL_GetError());
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);  // Gerar um ID de textura
    glBindTexture(GL_TEXTURE_2D, textureID);  // Bind a textura como textura 2D

    // Definir parâmetros de textura (opcional)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Especificar a imagem da textura
    GLenum texture_format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, texture_format, surface->w, surface->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);  // Liberar a superfície SDL, já que a textura foi criada

    return textureID;  // Retornar o ID da textura
}

void GAME_WINDOW::config_triangle_shader(float vertices[], int num_vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, num_vertices, vertices, GL_STATIC_DRAW);

    // Specifying how OpenGL should interpret data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // talvez isso de erros
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // create shaders
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    }
    )";

    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // orange
    }
    )";

   shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);
}

void GAME_WINDOW::draw_triangle(int num_vertices) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices / 2);
}

void GAME_WINDOW::config_square_shader(float vertices[], int num_vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(float), vertices, GL_STATIC_DRAW);

    // config like OpenGL should interprets data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind (opcional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // o shader pode ser o mesmo do que faz o triangulo
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    }
    )";

    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // orange
    }
    )";

   shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);
}

void GAME_WINDOW::config_image_shader(float vertices[], int num_vertices, unsigned index[], int num_index) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind Vertex Array Object
    glBindVertexArray(VAO);

    // copy data from vertices to Buffer Vertices (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(float), vertices, GL_STATIC_DRAW);

    // copy data from index to Element Buffer (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_index * sizeof(unsigned), index, GL_STATIC_DRAW);

    // define position of vertices (attrib 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // define texture coordinates (attrib 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char* vertexShaderSource = load_shader("assets/shaders/image_vertex.glsl");
    const char* fragmentShaderSource = load_shader("assets/shaders/image_fragment.glsl");

    shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);
}

void GAME_WINDOW::draw_image(GLuint textureID, float x, float y, float w, float h, float theta) {
    VEC2 p1 = apply_rotation({x,     y},     {x + w / 2, y + h / 2}, theta);
    VEC2 p2 = apply_rotation({x + w, y},     {x + w / 2, y + h / 2},  theta);
    VEC2 p3 = apply_rotation({x + w, y + h}, {x + w / 2, y + h / 2}, theta);
    VEC2 p4 = apply_rotation({x,     y + h}, {x + w / 2, y + h / 2},  theta);


    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);   glVertex2f(p1.x, p1.y);   // top-left
    glTexCoord2f(1.0f, 0.0f);   glVertex2f(p2.x, p2.y);   // top-right
    glTexCoord2f(1.0f, 1.0f);   glVertex2f(p3.x, p3.y);   // bottom-right
    glTexCoord2f(0.0f, 1.0f);   glVertex2f(p4.x, p4.y);   // bottom-left
    glEnd();
}

VEC2 GAME_WINDOW::apply_rotation(VEC2 point, VEC2 center, float theta) {
    /*
    * x′=x⋅cos(θ)−y⋅sin(θ)
    * y′=x⋅sin⁡(θ)+y⋅cos⁡(θ)y′=x⋅sin(θ)+y⋅cos(θ)
    */

    float translated_x = point.x - center.x;
    float translated_y = point.y - center.y;

    float rotated_x = translated_x * cos(theta) - translated_y * sin(theta);
    float rotated_y = translated_x * sin(theta) + translated_y * cos(theta);

    return { rotated_x + center.x, rotated_y + center.y };
}