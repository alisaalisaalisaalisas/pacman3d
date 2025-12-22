#include "ghost.h"
#include "maze.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

Ghost::Ghost(GhostType type)
    : Entity()
    , ghost_type(type)
    , mode(GhostMode::CHASE)
    , target_tile(0, 0)
    , isEaten(false)
    , frightenedTime(0.0f)
    , spawn_x(1)
    , spawn_y(1)
{
    color = getGhostColor(type);
    switch (type) {
        case GhostType::BLINKY: move_duration = 0.20f; break;
        case GhostType::PINKY:  move_duration = 0.24f; break;
        case GhostType::INKY:   move_duration = 0.26f; break;
        case GhostType::CLYDE:  move_duration = 0.28f; break;
    }
}

glm::vec3 Ghost::getGhostColor(GhostType type) {
    switch (type) {
        case GhostType::BLINKY: return glm::vec3(1.0f, 0.0f, 0.0f);
        case GhostType::PINKY:  return glm::vec3(1.0f, 0.59f, 0.78f);
        case GhostType::INKY:   return glm::vec3(0.0f, 0.59f, 1.0f);
        case GhostType::CLYDE:  return glm::vec3(1.0f, 0.59f, 0.0f);
        default: return glm::vec3(1.0f, 1.0f, 1.0f);
    }
}

void Ghost::update(float delta_time) {
    if (isEaten) return;
    
    Entity::update(delta_time);
    
    if (mode == GhostMode::FRIGHTENED) {
        frightenedTime -= delta_time;
        if (frightenedTime <= 0.0f) {
            mode = GhostMode::CHASE;
        }
    }
}

void Ghost::setFrightened(float duration) {
    if (!isEaten) {
        mode = GhostMode::FRIGHTENED;
        frightenedTime = duration;
        move_duration *= 1.5f; // Slower when frightened
    }
}

void Ghost::respawn(const Maze& maze, int x, int y) {
    spawn_x = x;
    spawn_y = y;
    setGridPosition(x, y, maze);
    isEaten = false;
    mode = GhostMode::CHASE;
    frightenedTime = 0.0f;
    current_dir = Direction::NONE;
    
    switch (ghost_type) {
        case GhostType::BLINKY: move_duration = 0.20f; break;
        case GhostType::PINKY:  move_duration = 0.24f; break;
        case GhostType::INKY:   move_duration = 0.26f; break;
        case GhostType::CLYDE:  move_duration = 0.28f; break;
    }
}

void Ghost::updateAI(const Maze& maze, const glm::ivec2& pacman_pos) {
    if (isEaten || is_moving) return;
    
    if (mode == GhostMode::FRIGHTENED) {
        // Run away from Pac-Man
        target_tile = glm::ivec2(grid_x * 2 - pacman_pos.x, grid_y * 2 - pacman_pos.y);
    }
    else if (mode == GhostMode::CHASE) {
        switch (ghost_type) {
            case GhostType::BLINKY:
                target_tile = pacman_pos;
                break;
            case GhostType::PINKY:
                target_tile = pacman_pos + glm::ivec2(4, 0);
                break;
            case GhostType::INKY:
                target_tile = pacman_pos + glm::ivec2(-3, 3);
                break;
            case GhostType::CLYDE:
                {
                    int dist = manhattanDistance(glm::ivec2(grid_x, grid_y), pacman_pos);
                    target_tile = (dist > 8) ? pacman_pos : glm::ivec2(1, 1);
                }
                break;
        }
    }
    
    Direction best_dir = findBestDirection(maze);
    if (best_dir != Direction::NONE) {
        tryMove(best_dir, maze);
    }
}

Direction Ghost::findBestDirection(const Maze& maze) {
    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    
    Direction best_dir = Direction::NONE;
    int best_dist = (mode == GhostMode::FRIGHTENED) ? -999999 : 999999;
    
    Direction opposite = Direction::NONE;
    switch (current_dir) {
        case Direction::UP:    opposite = Direction::DOWN; break;
        case Direction::DOWN:  opposite = Direction::UP; break;
        case Direction::LEFT:  opposite = Direction::RIGHT; break;
        case Direction::RIGHT: opposite = Direction::LEFT; break;
        default: break;
    }
    
    for (Direction dir : directions) {
        if (dir == opposite) continue;
        
        glm::ivec2 offset = getDirectionOffset(dir);
        int new_x = grid_x + offset.x;
        int new_y = grid_y + offset.y;
        
        if (!maze.isWalkable(new_x, new_y)) continue;
        
        int dist = manhattanDistance(glm::ivec2(new_x, new_y), target_tile);
        
        if (mode == GhostMode::FRIGHTENED) {
            if (dist > best_dist) { best_dist = dist; best_dir = dir; }
        } else {
            if (dist < best_dist) { best_dist = dist; best_dir = dir; }
        }
    }
    
    return best_dir;
}

int Ghost::manhattanDistance(glm::ivec2 a, glm::ivec2 b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

void Ghost::onTileReached() {
}

void Ghost::render(Shader& shader) {
    if (isEaten) return;
    
    glm::vec3 render_color = (mode == GhostMode::FRIGHTENED) ? FRIGHTENED_COLOR : color;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, world_pos);
    model = glm::scale(model, glm::vec3(0.8f));
    
    shader.setMat4("model", model);
    mesh.draw();
}

void Ghost::createMesh() {
    mesh = createCube(color);
    frightenedMesh = createCube(FRIGHTENED_COLOR);
}
