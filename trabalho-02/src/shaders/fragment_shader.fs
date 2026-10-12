#version 330 core

in vec2 coordenadasTextura;
out vec4 FragColor;

uniform sampler2D imagem;

void main() {
    vec4 cor = texture(imagem, coordenadasTextura);
    if (cor.a < 0.5) discard;
    FragColor = vec4(cor.rgb, 1.0);
}