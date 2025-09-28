#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>
#include <stdbool.h>

SDL_Window* window = NULL;

bool programRunning = true;

void main_loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            programRunning = false;
        }
    }

    // Clear the screen to black
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle from the 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);
}

int main(int argc, char** argv) {
    // SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    auto glc = SDL_GL_CreateContext(window);

    auto rdr = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //TODO
    printf("\n");
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION)); //hier ist es wohl ok, wenn was hoeheres raus kommt als 2.0es oder was auch immer
    printf("Shading Language Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


    // Create and compile the vertex shader
    const GLchar* vertexSource = SDL_LoadFile("shaders/maticurve-vertexshader.glsl", NULL);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    const GLchar* fragmentSource = SDL_LoadFile("shaders/maticurve-fragmentshader.glsl", NULL);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);




    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    GLfloat vertices[] = {0.0f, 0.5f,
                          0.5f, -0.5f,
                          -0.5f, -0.5f};
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // noch einen fuer andere Daten
    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    GLfloat verticesKurvenInfo[] = {1.0f, 1.0f,
                          0.0f, 1.0f,
                          1.0f, 0.0f};
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesKurvenInfo), verticesKurvenInfo, GL_STATIC_DRAW);
    GLint kurvenInfoAttrib = glGetAttribLocation(shaderProgram, "kurvenInfo");
    glEnableVertexAttribArray(kurvenInfoAttrib);
    glVertexAttribPointer(kurvenInfoAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // noch einen fuer Farbe 1
    GLuint vbo3;
    glGenBuffers(1, &vbo3);
    GLfloat verticesColorInside[] = {1.0f, 1.0f, 0.0f, 1.0f,
                          1.0f, 1.0f, 0.0f, 1.0f,
                          1.0f, 1.0f, 0.0f, 1.0f};
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorInside), verticesColorInside, GL_STATIC_DRAW);
    GLint colorInsideAttrib = glGetAttribLocation(shaderProgram, "colorInside");
    glEnableVertexAttribArray(colorInsideAttrib);
    glVertexAttribPointer(colorInsideAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // noch einen fuer Farbe 2
    GLuint vbo4;
    glGenBuffers(1, &vbo4);
    GLfloat verticesColorOutside[] = {0.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 0.0f};
    glBindBuffer(GL_ARRAY_BUFFER, vbo4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorOutside), verticesColorOutside, GL_STATIC_DRAW);
    GLint colorOutsideAttrib = glGetAttribLocation(shaderProgram, "colorOutside");
    glEnableVertexAttribArray(colorOutsideAttrib);
    glVertexAttribPointer(colorOutsideAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    GLint windowSizeUniform = glGetUniformLocation(shaderProgram, "u_windowSizeInPixel");
    glUniform2f(windowSizeUniform, 640.0f, 480.0f);

    while (programRunning) {
        main_loop();
        SDL_Delay(100);
    }

    //TODO do more cleanup
    SDL_free(vertexSource);
    SDL_free(fragmentSource);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
