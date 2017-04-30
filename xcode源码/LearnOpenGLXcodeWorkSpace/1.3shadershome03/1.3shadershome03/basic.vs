#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourPosition;

void main()
{
    ourPosition = position;
    gl_Position = vec4(position.x, position.y, position.z, 1.0f); // Just add a - to the y position
}
