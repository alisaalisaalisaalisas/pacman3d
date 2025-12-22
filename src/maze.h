#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

/**
 * Tile types for the maze grid.
 */
enum class TileType {
    EMPTY,      // Empty space (outside maze, tunnels)
    WALL,       // Solid wall block
    FLOOR,      // Walkable floor
    PELLET,     // Floor with pellet
    POWER,      // Floor with power pellet
    DOOR        // Ghost house door
};

/**
 * Maze class for loading and managing the game grid.
 * Converts 2D grid coordinates to 3D world positions.
 */
class Maze {
public:
    Maze();
    
    // Load maze from text file
    bool load(const std::string& filepath);
    
    // Get tile at grid position
    TileType getTile(int x, int y) const;
    
    // Set tile at grid position
    void setTile(int x, int y, TileType type);
    
    // Convert grid position to world position
    glm::vec3 gridToWorld(int x, int y) const;
    
    // Convert world position to grid position
    glm::ivec2 worldToGrid(const glm::vec3& pos) const;
    
    // Check if position is walkable
    bool isWalkable(int x, int y) const;
    
    // Maze dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    // Get center of maze in world coordinates
    glm::vec3 getCenter() const;
    
    // Tile size in world units
    static constexpr float TILE_SIZE = 1.0f;

private:
    int width;
    int height;
    std::vector<TileType> tiles;
    
    // Convert character to tile type
    TileType charToTile(char c) const;
};

#endif // MAZE_H
