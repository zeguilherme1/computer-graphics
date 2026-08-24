#version 330 core

layout (location = 0) in vec3 position;

void main()
{
    float angle = radians(45.0);

    mat3 rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, cos(angle), -sin(angle),
        0.0, sin(angle),  cos(angle)
    );

    vec3 rotatedPosition = rotation * position;

    gl_Position = vec4(rotatedPosition, 1.0);
}