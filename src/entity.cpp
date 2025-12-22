#include "entity.h"
#include "maze.h"
#include <algorithm>

Entity::Entity()
    : grid_x(0)
    , grid_y(0)
    , world_pos(0.0f)
    , current_dir(Direction::NONE)
    , desired_dir(Direction::NONE)
    , move_duration(0.18f)
    , move_elapsed(0.0f)
    , is_moving(false)
    , prev_pos(0.0f)
    , target_pos(0.0f)
{}

void Entity::update(float delta_time) {
    if (is_moving) {
        move_elapsed += delta_time;
        
        // Calculate interpolation factor
        float t = std::min(move_elapsed / move_duration, 1.0f);
        
        // Smooth interpolation (ease-out)
        t = 1.0f - (1.0f - t) * (1.0f - t);
        
        // Interpolate position
        world_pos = glm::mix(prev_pos, target_pos, t);
        
        // Check if movement complete
        if (move_elapsed >= move_duration) {
            world_pos = target_pos;
            is_moving = false;
            move_elapsed = 0.0f;
            onTileReached();
        }
    }
}

void Entity::setGridPosition(int x, int y, const Maze& maze) {
    grid_x = x;
    grid_y = y;
    world_pos = maze.gridToWorld(x, y);
    world_pos.y = 0.5f; // Slightly above floor
    prev_pos = world_pos;
    target_pos = world_pos;
    is_moving = false;
    move_elapsed = 0.0f;
}

bool Entity::tryMove(Direction dir, const Maze& maze) {
    if (is_moving) return false;
    if (dir == Direction::NONE) return false;
    
    glm::ivec2 offset = getDirectionOffset(dir);
    int new_x = grid_x + offset.x;
    int new_y = grid_y + offset.y;
    
    // Check if new position is walkable
    if (!maze.isWalkable(new_x, new_y)) {
        return false;
    }
    
    // Start movement
    grid_x = new_x;
    grid_y = new_y;
    current_dir = dir;
    
    prev_pos = world_pos;
    target_pos = maze.gridToWorld(new_x, new_y);
    target_pos.y = 0.5f;
    
    is_moving = true;
    move_elapsed = 0.0f;
    
    return true;
}

glm::ivec2 Entity::getDirectionOffset(Direction dir) {
    switch (dir) {
        case Direction::UP:    return glm::ivec2(0, 1);
        case Direction::DOWN:  return glm::ivec2(0, -1);
        case Direction::LEFT:  return glm::ivec2(-1, 0);
        case Direction::RIGHT: return glm::ivec2(1, 0);
        default:               return glm::ivec2(0, 0);
    }
}
