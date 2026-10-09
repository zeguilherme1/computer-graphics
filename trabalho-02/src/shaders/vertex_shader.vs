#version 330 core

in vec3 position;
in vec2 texture_coord;

out vec2 coordenadasTextura;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    coordenadasTextura = texture_coord;
}