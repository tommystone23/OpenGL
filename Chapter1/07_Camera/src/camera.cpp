#include "camera.h"

Camera::Camera(int windowX, int windowY)
{
    delta_time = 0.0f; // Time between current frame and last frame
    last_frame = 0.0f; // Time of last frame

    yaw = 0.0f; // Up and down mouse movement
    pitch = 0.0f; // Side to side mouse movement

    zoom = 45.0f; // How zoomed in the perspective is

    camera_pos = glm::vec3(0.0f, 0.0f, 4.0f);
    camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    camera_direction = glm::normalize(camera_pos - camera_target);

    // Setup right axis
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera_right = glm::normalize(glm::cross(up, camera_direction));

    // Setup up axis
    camera_up = glm::cross(camera_direction, camera_right);

    camera_front = glm::vec3(0.5f, 0.5f, -1.0f);

    camera_speed = 0.5f;

    mouseX = 0;
    mouseY = windowY / 2;
    mouse_pressed = false;
    lastX = windowX / 2;
    lastY = windowY / 2;

    scrollwheel_offset = 45;

    view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    projection = glm::perspective(glm::radians(zoom), 800.0f / 600.0f, 0.1f, 100.0f);
}

Camera::~Camera()
{

}

void Camera::mouse_callback()
{
    float x_offset = (float)mouseX - lastX;
    float y_offset = (float)lastY - mouseY; // reversed: y ranges bottom to top
    lastX = (float)mouseX;
    lastY = (float)mouseY;

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

void Camera::scroll_callback()
{
    zoom -= (float)scrollwheel_offset;

    if(zoom < 1.0f)
        zoom = 1.0f;
    if(zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::set_mouse_coords(int mouseX, int mouseY)
{
    this->mouseX += mouseX;
    this->mouseY += mouseY;
}

void Camera::calc_mouse_look_direction()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}

bool Camera::check_mouse_pressed()
{
    return mouse_pressed;
}

void Camera::set_mouse_pressed(bool pressed)
{
    this->mouse_pressed = pressed;
}

void Camera::set_scrollwheel_offset(int offset)
{
    this->scrollwheel_offset = offset;
}

void Camera::set_camera_pos(char key)
{
    switch(key)
    {
        case('w'):
            this->camera_pos += camera_speed * camera_front;
            break;
        case('a'):
            this->camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
            break;
        case('s'):
            this->camera_pos -= camera_speed * camera_front;
            break;
        case('d'):
            this->camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
            break;

    }
}

void Camera::sync_camera_frames()
{
    // This ensures that camera speed is the same regardless of computer speed
    float current_frame = SDL_GetTicks64();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    // Multiply by delta time to ensure speed remains constant on all systems
    camera_speed = 0.03f * delta_time;
}

void Camera::update_view()
{
    view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

void Camera::update_projection()
{
    projection = glm::perspective(glm::radians(zoom), 800.0f / 600.0f, 0.1f, 100.0f);
}

glm::mat4 Camera::get_view()
{
    return this->view;
}

glm::mat4 Camera::get_projection()
{
    return this->projection;
}