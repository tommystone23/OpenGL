#include <stdio.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

int main()
{
    // Setup SDL Stuff
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL");
        return -1;
    }

    // Setup OpenGL Attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create Window
    SDL_Window* window = SDL_CreateWindow("Learn OpenGL", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window)
    {
        printf("Failed to create SDL window");
        return -1;
    }

    // Create Context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        printf("Failed to create context");
        return -1;
    }

    // Make Current Context
    SDL_GL_MakeCurrent(window, context);

    // Create View Port
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        printf("Error initializing GLEW %s\n", glewGetErrorString(glewError));
        return -1;
    }

    // Event Loop
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case(SDL_QUIT):
                    quit = true;
                    break;
            }
        }
        // Set background colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap windows
        SDL_GL_SwapWindow(window);
    }

    // SDL Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}