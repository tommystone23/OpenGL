#include <stdio.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "shaders.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 sharedColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = sharedColor;\n"
    "}\0";

const char *vertexShaderSourceTopLeft = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 topLeftColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "topLeftColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);\n"
    "}\0";

const char *fragmentShaderSourceTopLeft = "#version 330 core\n"
    "in vec4 topLeftColor;"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = topLeftColor;\n"
    "}\0";

const char *vertexShaderSourceTopRight = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n" // Position has atrribute location 0
    "layout (location = 1) in vec3 aColor;\n" // Color has attribute position 1
    "out vec3 topRightColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "topRightColor = aColor;\n" // Set color to be from vertex data
    "}\0";

const char *fragmentShaderSourceTopRight = "#version 330 core\n"
    "in vec3 topRightColor;"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(topRightColor, 1.0f);\n"
    "}\0";

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

    // Create shader object
    Shader myShader("/home/tommy/development/learn_opengl_tutorials/03_Shaders/shaders/triangle_bRight.vertex", "/home/tommy/development/learn_opengl_tutorials/03_Shaders/shaders/triangle_bRight.frag");

    // Triangle Vertices
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 
        -0.5f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };

    float topLeftVertices[] = {
        -0.5f, 0.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    float topRightVertices[] = {
        0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // First 3 vertices, last 3 is colour
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f
    };
    
    float bottomRightVertices[] = {
        0.5f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    // Compile Shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int topLeftVertexShader;
    topLeftVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(topLeftVertexShader, 1, &vertexShaderSourceTopLeft, NULL);
    glCompileShader(topLeftVertexShader);

    unsigned int topLeftFragmentShader;
    topLeftFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(topLeftFragmentShader, 1, &fragmentShaderSourceTopLeft, NULL);
    glCompileShader(topLeftFragmentShader);

    unsigned int topRightVertexShader;
    topRightVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(topRightVertexShader, 1, &vertexShaderSourceTopRight, NULL);
    glCompileShader(topRightVertexShader);

    unsigned int topRightFragmentShader;
    topRightFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(topRightFragmentShader, 1, &fragmentShaderSourceTopRight, NULL);
    glCompileShader(topRightFragmentShader);



    // Check Shader Compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX(bottom_left)::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT(bottom_left)::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    glGetShaderiv(topLeftVertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(topLeftVertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX(top_left)::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    glGetShaderiv(topLeftFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(topLeftFragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT(top_left)::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    glGetShaderiv(topRightVertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(topRightVertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX(top_right)::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    glGetShaderiv(topRightFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(topRightFragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT(top_right)::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    // Create Shader Program (Link Shaders Together)
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    unsigned int topLeftShaderProgram;
    topLeftShaderProgram = glCreateProgram();
    glAttachShader(topLeftShaderProgram, topLeftVertexShader);
    glAttachShader(topLeftShaderProgram, topLeftFragmentShader);
    glLinkProgram(topLeftShaderProgram);

    unsigned int topRightShaderProgram;
    topRightShaderProgram = glCreateProgram();
    glAttachShader(topRightShaderProgram, topRightVertexShader);
    glAttachShader(topRightShaderProgram, topRightFragmentShader);
    glLinkProgram(topRightShaderProgram);


    // Check Program Linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM:LINKING_FAILED: %s", infoLog);
        return -1;
    }

    glGetProgramiv(topLeftShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(topLeftShaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM:LINKING_FAILED: %s", infoLog);
        return -1;
    }

    glGetProgramiv(topRightShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(topRightShaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM:LINKING_FAILED: %s", infoLog);
        return -1;
    }

    // Cleanup Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(topLeftVertexShader);
    glDeleteShader(topLeftFragmentShader);
    glDeleteShader(topRightVertexShader);
    glDeleteShader(topRightFragmentShader);

    // Note: bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    // Setup Vertex Buffer Object and Vertex Array Object
    unsigned int VBO[4];
    unsigned int VAO[4];
    glGenVertexArrays(4, VAO);
    glGenBuffers(4, VBO);

    // Link Vertex Attributes with VAO
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(topLeftVertices), topLeftVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(topRightVertices), topRightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Assign color values in vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottomRightVertices), bottomRightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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

        // Define program, VAO, then draw (This is drawing workflow)
        glUseProgram(shaderProgram);
        float timeValue = SDL_GetTicks64() / 1000;
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "sharedColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(topLeftShaderProgram);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(topRightShaderProgram);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Use shader program, set uniform float values in shaders
        myShader.use();
        myShader.setFloat("red", 0.5f);
        myShader.setFloat("green", 0.0f);
        myShader.setFloat("blue", 1.0f);
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Swap windows
        SDL_GL_SwapWindow(window);
    }

    /* Cleanup created OpenGL objects. */
    glDeleteVertexArrays(4, VAO);
    glDeleteBuffers(4, VBO);

    // SDL Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}