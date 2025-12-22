#ifndef GHOST_H
#define GHOST_H

#include "entity.h"
#include "mesh.h"
#include "shader.h"

enum class GhostMode {
    CHASE,
    SCATTER,
    FRIGHTENED
};

enum class GhostType {
    BLINKY = 0,
    PINKY = 1,
    INKY = 2,
    CLYDE = 3
};

class Ghost : public Entity {
public:
    Ghost(GhostType type);
    
    GhostType ghost_type;
    GhostMode mode;
    glm::vec3 color;
    glm::ivec2 target_tile;
    
    bool isEaten;
    float frightenedTime;
    
    void update(float delta_time) override;
    void updateAI(const class Maze& maze, const glm::ivec2& pacman_pos);
    void render(Shader& shader);
    void createMesh();
    
    void setFrightened(float duration);
    void respawn(const class Maze& maze, int x, int y);
    
    static glm::vec3 getGhostColor(GhostType type);
    static constexpr glm::vec3 FRIGHTENED_COLOR{0.3f, 0.3f, 1.0f};
    
private:
    Mesh mesh;
    Mesh frightenedMesh;
    int spawn_x, spawn_y;
    
    Direction findBestDirection(const class Maze& maze);
    int manhattanDistance(glm::ivec2 a, glm::ivec2 b);
    void onTileReached() override;
};

#endif // GHOST_H
