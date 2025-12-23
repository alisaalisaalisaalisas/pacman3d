#include "../glad/glad.h"
#include "SpriteManager.h"
#include "SpriteData.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

SpriteManager::SpriteManager() : quadVAO(0), quadVBO(0) {}

SpriteManager::~SpriteManager() {
    shutdown();
}

bool SpriteManager::init() {
    if (!spriteShader.load("shaders/sprite_vertex.glsl", "shaders/sprite_fragment.glsl")) {
        std::cerr << "Failed to load sprite shader" << std::endl;
        return false;
    }
    
    // Load new sprite files
    if (!pacmanTexture.load("assets/sprites/collectibles.png")) {
        std::cerr << "Warning: pacman texture not found" << std::endl;
    }
    if (!ghostTexture.load("assets/sprites/ghosts.png")) {
        std::cerr << "Warning: ghost texture not found" << std::endl;
    }
    if (!collectiblesTexture.load("assets/sprites/collectibles.png")) {
        std::cerr << "Warning: collectibles texture not found" << std::endl;
    }
    
    setupQuad();
    std::cout << "Sprites initialized" << std::endl;
    return true;
}

void SpriteManager::shutdown() {
    if (quadVBO) glDeleteBuffers(1, &quadVBO);
    if (quadVAO) glDeleteVertexArrays(1, &quadVAO);
    quadVAO = quadVBO = 0;
}

void SpriteManager::setupQuad() {
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void SpriteManager::renderSprite(const Texture& tex, float u1, float v1, float u2, float v2,
                                  const glm::vec3& pos, float size, const glm::mat4& view, const glm::mat4& proj) {
    float vertices[] = {
        -0.5f, 0.0f, 0.0f,  u1, v1,
         0.5f, 0.0f, 0.0f,  u2, v1,
         0.5f, 1.0f, 0.0f,  u2, v2,
        -0.5f, 0.0f, 0.0f,  u1, v1,
         0.5f, 1.0f, 0.0f,  u2, v2,
        -0.5f, 1.0f, 0.0f,  u1, v2
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glm::vec3 camRight = glm::vec3(view[0][0], view[1][0], view[2][0]);
    glm::vec3 camUp = glm::vec3(0, 1, 0);
    
    glm::mat4 model = glm::mat4(1.0f);
    model[0] = glm::vec4(camRight * size, 0.0f);
    model[1] = glm::vec4(camUp * size, 0.0f);
    model[2] = glm::vec4(glm::cross(camRight, camUp) * size, 0.0f);
    model[3] = glm::vec4(pos, 1.0f);
    
    spriteShader.use();
    spriteShader.setMat4("model", model);
    spriteShader.setMat4("view", view);
    spriteShader.setMat4("projection", proj);
    
    tex.bind(0);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteManager::renderPacMan(int direction, int frame, const glm::vec3& pos, 
                                  const glm::mat4& view, const glm::mat4& proj) {
    // Use yellow square from collectibles for now (placeholder until PacmanFinal.png)
    renderSprite(pacmanTexture, 0.0f, 0.0f, 0.1f, 0.1f, pos, 0.8f, view, proj);
}

void SpriteManager::renderGhost(int ghostType, int frame, const glm::vec3& pos,
                                 const glm::mat4& view, const glm::mat4& proj, bool frightened) {
    // ghosts.png has 4 ghosts in a row: cyan(0), orange(1), red(2), pink(3)
    // Map ghost types: RED=0->2, BLUE=1->0, ORANGE=2->1, PINK=3->3
    int spriteIndex = ghostType;
    switch (ghostType) {
        case 0: spriteIndex = 2; break; // RED -> position 2
        case 1: spriteIndex = 3; break; // BLUE -> use pink for now (position 3)
        case 2: spriteIndex = 1; break; // ORANGE -> position 1
        case 3: spriteIndex = 3; break; // PINK -> position 3
    }
    
    float w = static_cast<float>(ghostTexture.getWidth());
    float ghostWidth = w / 4.0f;
    
    float u1 = (spriteIndex * ghostWidth) / w;
    float u2 = ((spriteIndex + 1) * ghostWidth) / w;
    
    renderSprite(ghostTexture, u1, 0.0f, u2, 1.0f, pos, 0.9f, view, proj);
}

void SpriteManager::renderPellet(const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj) {
    renderSprite(collectiblesTexture, 0.0f, 0.0f, 0.05f, 0.05f, pos, 0.2f, view, proj);
}

void SpriteManager::renderPowerUp(int frame, const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj) {
    renderSprite(collectiblesTexture, 0.0f, 0.0f, 0.1f, 0.1f, pos, 0.4f, view, proj);
}