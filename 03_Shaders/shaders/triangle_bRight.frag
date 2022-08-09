#version 330 core
out vec4 fragColor;
uniform float red;
uniform float green;
uniform float blue;
void main()
{
    fragColor = vec4(red, green, blue, 1.0f);
}