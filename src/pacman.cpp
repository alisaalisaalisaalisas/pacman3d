#include "pacman.h"
#include "maze.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

PacMan::PacMan()
    : Entity()
    , score(0)
    , pelletsEaten(0)
    , lives(3)
    , isDead(false)
    , mouth_angle(0.0f)
    , mouth_speed(8.0f)
    , mouth_opening(true)
    , powerTime(0.0f)
    , isPowered(false)
    , buffered_dir(Direction::NONE)
    , spawn_x(14)
    , spawn_y(6)
{
    move_duration = 0.18f;
}

void PacMan::update(float delta_time) {
    if (isDead) return;
    
    Entity::update(delta_time);
    
    // Power pellet timer
    if (isPowered) {
        powerTime -= delta_time;
        if (powerTime <= 0.0f) {
            isPowered = false;
            std::cout << "Power wore off!" << std::endl;
        }
    }
    
    if (is_moving || current_dir != Direction::NONE) {
        if (mouth_opening) {
            mouth_angle += mouth_speed * delta_time;
            if (mouth_angle >= 0.5f) {
                mouth_angle = 0.5f;
                mouth_opening = false;
            }
        } else {
            mouth_angle -= mouth_speed * delta_time;
            if (mouth_angle <= 0.0f) {
                mouth_angle = 0.0f;
                mouth_opening = true;
            }
        }
    }
}

void PacMan::handleInput(Direction input_dir, const Maze& maze) {
    if (isDead) return;
    if (input_dir == Direction::NONE) return;
    
    buffered_dir = input_dir;
    desired_dir = input_dir;
    
    if (!is_moving) {
        if (!tryMove(input_dir, maze)) {
            if (current_dir != Direction::NONE) {
                tryMove(current_dir, maze);
            }
        }
    }
}

void PacMan::continueMovement(const Maze& maze) {
    if (isDead) return;
    if (is_moving) return;
    
    if (buffered_dir != Direction::NONE) {
        if (tryMove(buffered_dir, maze)) {
            return;
        }
    }
    
    if (current_dir != Direction::NONE) {
        tryMove(current_dir, maze);
    }
}

bool PacMan::collectPellet(Maze& maze) {
    if (isDead) return false;
    
    TileType tile = maze.getTile(grid_x, grid_y);
    
    if (tile == TileType::PELLET) {
        maze.setTile(grid_x, grid_y, TileType::FLOOR);
        score += 10;
        pelletsEaten++;
        return false;
    }
    else if (tile == TileType::POWER) {
        maze.setTile(grid_x, grid_y, TileType::FLOOR);
        score += 50;
        pelletsEaten++;
        isPowered = true;
        powerTime = POWER_DURATION;
        std::cout << "POWER UP! Ghosts are scared!" << std::endl;
        return true;
    }
    return false;
}

void PacMan::die() {
    lives--;
    isDead = true;
    std::cout << "Pac-Man died! Lives remaining: " << lives << std::endl;
}

void PacMan::respawn(const Maze& maze) {
    isDead = false;
    setGridPosition(spawn_x, spawn_y, maze);
    current_dir = Direction::NONE;
    buffered_dir = Direction::NONE;
    isPowered = false;
    powerTime = 0.0f;
}

void PacMan::onTileReached() {
}

void PacMan::render(Shader& shader) {
    if (isDead) return;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, world_pos);
    
    float rotation = 0.0f;
    switch (current_dir) {
        case Direction::RIGHT: rotation = 0.0f; break;
        case Direction::UP:    rotation = 90.0f; break;
        case Direction::LEFT:  rotation = 180.0f; break;
        case Direction::DOWN:  rotation = 270.0f; break;
        default: break;
    }
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f));
    
    shader.setMat4("model", model);
    mesh.draw();
}

void PacMan::createMesh() {
    mesh = createCube(COLOR);
}
