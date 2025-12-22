#version 330 core

in vec3 fragNormal;
in vec3 fragColor;
in vec3 fragPos;

out vec4 FragColor;

// Directional light pointing from top-right
uniform vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));

// Ambient lighting strength (0.6 for bright voxel look)
uniform float ambientStrength = 0.6;

void main() {
    // Ambient lighting
    vec3 ambient = ambientStrength * fragColor;
    
    // Diffuse lighting
    vec3 norm = normalize(fragNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * fragColor;
    
    // Combine ambient and diffuse (no specular for matte voxel look)
    vec3 result = ambient + diffuse;
    
    FragColor = vec4(result, 1.0);
}
