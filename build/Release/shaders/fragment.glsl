#version 330 core

in vec3 fragNormal;
in vec3 fragColor;
in vec3 fragPos;
in vec2 fragTexCoord;

out vec4 FragColor;

uniform vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));
uniform float ambientStrength = 0.6;
uniform vec3 colorTint = vec3(1.0, 1.0, 1.0);
uniform sampler2D textureSampler;
uniform bool useTexture = false;

void main() {
    vec3 baseColor;
    
    if (useTexture) {
        vec4 texColor = texture(textureSampler, fragTexCoord);
        baseColor = texColor.rgb * colorTint;
    } else {
        baseColor = fragColor * colorTint;
    }
    
    vec3 ambient = ambientStrength * baseColor;
    
    vec3 norm = normalize(fragNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * baseColor;
    
    vec3 result = ambient + diffuse;
    
    FragColor = vec4(result, 1.0);
}
