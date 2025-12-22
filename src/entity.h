#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

/**
 * Direction enum for entity movement
 */
enum class Direction {
    NONE = -1,
    UP = 0,     // +Z
    DOWN = 1,   // -Z
    LEFT = 2,   // -X
    RIGHT = 3   // +X
};

/**
 * Base Entity class for all game entities (Pac-Man, Ghosts).
 * Handles tile-based movement with smooth interpolation.
 */
class Entity {
public:
    Entity();
    virtual ~Entity() = default;
    
    // Grid position
    int grid_x;
    int grid_y;
    
    // World position (interpolated)
    glm::vec3 world_pos;
    
    // Movement
    Direction current_dir;
    Direction desired_dir;
    
    // Movement timing
    float move_duration;    // Time to move one tile (0.18 sec)
    float move_elapsed;     // Current movement progress
    bool is_moving;
    
    // Previous position for interpolation
    glm::vec3 prev_pos;
    glm::vec3 target_pos;
    
    // Update entity (call each frame with delta time)
    virtual void update(float delta_time);
    
    // Set grid position immediately
    void setGridPosition(int x, int y, const class Maze& maze);
    
    // Try to move in a direction (returns true if movement started)
    bool tryMove(Direction dir, const class Maze& maze);
    
    // Get direction offset
    static glm::ivec2 getDirectionOffset(Direction dir);
    
protected:
    // Called when arriving at a new tile
    virtual void onTileReached() {}
};

#endif // ENTITY_H
