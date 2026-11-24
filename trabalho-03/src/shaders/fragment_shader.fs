#version 330 core

#define NUM_LIGHTS 3

in vec2 coordenadasTextura;
in vec3 fragPos;
in vec3 fragNormal;
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 color;
    int on;
    int env;      
};

uniform sampler2D imagem;
uniform Light lights[NUM_LIGHTS];
uniform vec3 viewPos;

//controle da luz
uniform float ambientLevel;
uniform float diffuseLevel;
uniform float specularLevel;

//parametros por objeto
uniform float matKa, matKd, matKs, matShininess;
uniform int objEnv;      
uniform vec3 emissive;
uniform int unlit;       

void main() {
    vec4 tex = texture(imagem, coordenadasTextura);
    if (tex.a < 0.5) discard;

    if (unlit == 1) {
        FragColor = vec4(tex.rgb, 1.0);
        return;
    }

    vec3 N = normalize(fragNormal);
    vec3 V = normalize(viewPos - fragPos);
    if (dot(N, V) < 0.0) N = -N;   

    vec3 diffuseSum = vec3(0.0);
    vec3 specularSum = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; i++) {
        if (lights[i].on == 0) continue;
        if (objEnv != 2 && objEnv != lights[i].env) continue;

        vec3 L = lights[i].position - fragPos;
        float d = length(L);
        L /= d;

        float att = 1.0 / (1.0 + 0.22 * d + 0.20 * d * d);

        float diff = max(dot(N, L), 0.0);
        diffuseSum += att * diff * lights[i].color;

        if (diff > 0.0) {
            vec3 R = reflect(-L, N);
            float spec = pow(max(dot(V, R), 0.0), matShininess);
            specularSum += att * spec * lights[i].color;
        }
    }

    vec3 ambient  = ambientLevel * matKa * vec3(1.0);
    vec3 diffuse  = diffuseLevel * matKd * diffuseSum;
    vec3 specular = specularLevel * matKs * specularSum;

    vec3 color = tex.rgb * (ambient + diffuse) + specular + emissive;
    FragColor = vec4(min(color, vec3(1.0)), 1.0);
}