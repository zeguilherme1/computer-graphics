#version 330 core

in vec3 position;
in vec2 texture_coord;
in vec3 normal;

out vec2 coordenadasTextura;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 world = model * vec4(position, 1.0);
    gl_Position = projection * view * world;
    coordenadasTextura = texture_coord;
    fragPos = world.xyz;
    fragNormal = mat3(transpose(inverse(model))) * normal;
}