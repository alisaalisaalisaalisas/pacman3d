#ifndef PACMAN_H
#define PACMAN_H

#include "entity.h"
#include "mesh.h"
#include "shader.h"

class PacMan : public Entity {
public:
    PacMan();
    
    int score;
    int pelletsEaten;
    int lives;
    bool isDead;
    
    float mouth_angle;
    float mouth_speed;
    bool mouth_opening;
    
    // Power pellet timer
    float powerTime;
    bool isPowered;
    
    void update(float delta_time) override;
    void handleInput(Direction input_dir, const class Maze& maze);
    void continueMovement(const class Maze& maze);
    bool collectPellet(class Maze& maze);  // Returns true if power pellet
    void die();
    void respawn(const class Maze& maze);
    void render(Shader& shader);
    void createMesh();
    
    static constexpr glm::vec3 COLOR{1.0f, 1.0f, 0.0f};
    static constexpr float POWER_DURATION = 8.0f;
    
private:
    Mesh mesh;
    Direction buffered_dir;
    int spawn_x, spawn_y;
    void onTileReached() override;
};

#endif // PACMAN_H
