#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "texture.h"
#include "shader.h"
#include <glm/glm.hpp>

class SpriteManager {
public:
    SpriteManager();
    ~SpriteManager();
    
    bool init();
    void shutdown();
    
    void renderPacMan(int direction, int frame, const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj);
    void renderGhost(int ghostType, int frame, const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj, bool frightened = false);
    void renderPellet(const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj);
    void renderPowerUp(int frame, const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj);
    
private:
    Texture pacmanTexture;
    Texture ghostTexture;
    Texture collectiblesTexture;
    Shader spriteShader;
    
    unsigned int quadVAO;
    unsigned int quadVBO;
    
    void setupQuad();
    void renderSprite(const Texture& tex, float u1, float v1, float u2, float v2,
                      const glm::vec3& pos, float size, const glm::mat4& view, const glm::mat4& proj);
};

#endif // SPRITE_MANAGER_H
