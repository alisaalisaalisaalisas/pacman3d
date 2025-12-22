#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragColor;
out vec3 fragPos;

void main() {
    // Transform vertex position to clip space
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    
    // Pass world position to fragment shader
    fragPos = vec3(model * vec4(aPosition, 1.0));
    
    // Transform normal to world space (using normal matrix)
    fragNormal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass vertex color through
    fragColor = aColor;
}
