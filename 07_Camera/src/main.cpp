#include <stdio.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "shaders.h"
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;

float delta_time = 0.0f; // Time between current frame and last frame
float last_frame = 0.0f; // Time of last frame

float yaw = 0.0f; // Up and down mouse movement
float pitch = 0.0f; // Side to side mouse movement

float zoom = 45.0f; // How zoomed in the perspective is

void mouse_callback(SDL_Window *window, int xPos, int yPos, float &lastX, float &lastY);
void scroll_callback(SDL_Window *window, int y_offset);

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
    Shader myShader("shaders/squareTexture.vertex", "shaders/squareTexture.fragment");

    // Generate Texture
    unsigned int texture0;
    glGenTextures(1, &texture0); // Assign ID
    glBindTexture(GL_TEXTURE_2D, texture0); // Bind ID to 2D texture
    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
    // Load and generate textures
    int width, height, nbChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nbChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        printf("Failed to load texture");

    unsigned int texture1;
    glGenTextures(1, &texture1); // Assign ID
    glBindTexture(GL_TEXTURE_2D, texture1); // Bind ID to 2D texture
    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("textures/awesomeface.png", &width, &height, &nbChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        printf("Failed to load texture");

    // Free texture data
    stbi_image_free(data);

    // Texture Coordinates (0,0) bottom left, (1,1) top right
    float vertices[] = {
        // vertex               // tex coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Note: bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    // Setup Buffer Object and Vertex Array Object
    unsigned int VBO[1];
    unsigned int VAO[1];
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // Link Vertex Attributes with VAO
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Setup camera
    // Camera direction
    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);

    // Setup right axis
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));

    // Setup up axis
    glm::vec3 camera_up = glm::cross(camera_direction, camera_right);

    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

    float camera_speed = 0.5f;

    // Mouse Input Parameters
    int mouseX, mouseY;
    bool mouse_pressed = false;
    float lastX = 960, lastY = 540;
    mouse_callback(window, mouseX, mouseY, lastX, lastY);

    int scrollwheel_offset = 45;

    // Set program and texture numbers
    myShader.use();
    myShader.setInt("texture0", 0);
    myShader.setInt("texture1", 1);

    // Makes the cubes look 3D
    glEnable(GL_DEPTH_TEST);

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
                case(SDL_KEYDOWN):
                    switch(event.key.keysym.sym)
                    {
                        case(SDLK_w):
                            camera_pos += camera_speed * camera_front;
                            break;
                        case(SDLK_a):
                            camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
                            break;
                        case(SDLK_s):
                            camera_pos -= camera_speed * camera_front;
                            break;
                        case(SDLK_d):
                            camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
                            break;
                        case(SDLK_ESCAPE):
                            quit = true;
                            break;
                    }
                // All mouse input for moving camera
                case(SDL_MOUSEBUTTONDOWN):
                    mouse_pressed = true;
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    break;
                case(SDL_MOUSEBUTTONUP):
                    mouse_pressed = false;
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    break;
                case(SDL_MOUSEMOTION):
                    if(mouse_pressed)
                    {
                        mouseX += event.motion.xrel;
                        mouseY += event.motion.yrel;
                    }
                    break;
                case(SDL_MOUSEWHEEL):
                    scrollwheel_offset = event.wheel.y;
                    scroll_callback(window, scrollwheel_offset);
            }
        }
        // Set background colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(mouse_pressed)
        {
            //SDL_GetRelativeMouseState((&mouseX, &mouseY));
            mouse_callback(window, mouseX, mouseY, lastX, lastY);
        }

        // This ensures that camera speed is the same regardless of computer speed
        float current_frame = SDL_GetTicks64();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        // Multiply by delta time to ensure speed remains constant on all systems
        camera_speed = 0.03f * delta_time;

        // Bind Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        /* Rotate camera around scene every second
        const float radius = 10.0f;
        float camX = sin(SDL_GetTicks64() / 1000) * radius;
        float camZ = cos(SDL_GetTicks64() / 1000) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        */

       // Mouse look direction
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera_front = glm::normalize(direction);



        // User input to move around camera
        glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);


        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        int viewLoc = glGetUniformLocation(myShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        for(int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            float angle = 20.0f + (i * 2);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::rotate(model, ((float)SDL_GetTicks() / 1000) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            int modelLoc = glGetUniformLocation(myShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // Swap windows
        SDL_GL_SwapWindow(window);
    }

    /* Cleanup created OpenGL objects. */
    glDeleteVertexArrays(1, VAO);

    // SDL Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void mouse_callback(SDL_Window *window, int xPos, int yPos, float &lastX, float &lastY)
{
    float x_offset = (float)xPos - lastX;
    float y_offset = (float)lastY - yPos; // reversed: y ranges bottom to top
    lastX = (float)xPos;
    lastY = (float)yPos;

    const float sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    // Constraints
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}

void scroll_callback(SDL_Window *window, int y_offset)
{
    zoom -= (float)y_offset;

    if(zoom < 1.0f)
        zoom = 1.0f;
    if(zoom > 45.0f)
        zoom = 45.0f;
}