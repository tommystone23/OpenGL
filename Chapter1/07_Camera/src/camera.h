#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{

private:
    float delta_time;
    float last_frame;

    float yaw;
    float pitch;

    float zoom;

    glm::vec3 camera_direction;
    glm::vec3 up;
    glm::vec3 camera_right;
    glm::vec3 camera_up;
    glm::vec3 camera_front;
    float camera_speed;

    glm::mat4 view;
    glm::mat4 projection;

    int mouseX;
    int mouseY;
    bool mouse_pressed;
    float lastX;
    float lastY;

    int scrollwheel_offset;

public:
    Camera(int windowX, int windowY);
    ~Camera();

    glm::vec3 camera_pos;
    glm::vec3 camera_target;
    void mouse_callback();
    void scroll_callback();
    void set_mouse_coords(int mouseX, int mouseY);
    void calc_mouse_look_direction();
    bool check_mouse_pressed();
    void sync_camera_frames();
    void update_view();
    void update_projection();
    void set_mouse_pressed(bool pressed);
    void set_scrollwheel_offset(int offset);
    void set_camera_pos(char key);

    glm::mat4 get_view();
    glm::mat4 get_projection();
};

