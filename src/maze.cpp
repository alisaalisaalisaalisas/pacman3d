#include "maze.h"
#include <fstream>
#include <iostream>
#include <algorithm>

Maze::Maze() : width(0), height(0) {}

bool Maze::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "ERROR::MAZE: Could not open file: " << filepath << std::endl;
        return false;
    }
    
    // Read all lines to determine dimensions
    std::vector<std::string> lines;
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    if (lines.empty()) {
        std::cerr << "ERROR::MAZE: Empty maze file" << std::endl;
        return false;
    }
    
    // Determine maze dimensions
    height = static_cast<int>(lines.size());
    width = 0;
    for (const auto& l : lines) {
        width = std::max(width, static_cast<int>(l.length()));
    }
    
    // Allocate tile array
    tiles.resize(width * height, TileType::EMPTY);
    
    // Parse tiles (flip Y so bottom of file is Y=0)
    for (int y = 0; y < height; ++y) {
        const std::string& row = lines[height - 1 - y];
        for (int x = 0; x < static_cast<int>(row.length()); ++x) {
            tiles[y * width + x] = charToTile(row[x]);
        }
    }
    
    std::cout << "Loaded maze: " << width << "x" << height << " tiles" << std::endl;
    return true;
}

TileType Maze::getTile(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return TileType::EMPTY;
    }
    return tiles[y * width + x];
}

void Maze::setTile(int x, int y, TileType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        tiles[y * width + x] = type;
    }
}

glm::vec3 Maze::gridToWorld(int x, int y) const {
    // Center tile (0,0) at world origin, Y is up
    return glm::vec3(
        (x - width / 2.0f + 0.5f) * TILE_SIZE,
        0.0f,
        (y - height / 2.0f + 0.5f) * TILE_SIZE
    );
}

glm::ivec2 Maze::worldToGrid(const glm::vec3& pos) const {
    return glm::ivec2(
        static_cast<int>(pos.x / TILE_SIZE + width / 2.0f),
        static_cast<int>(pos.z / TILE_SIZE + height / 2.0f)
    );
}

bool Maze::isWalkable(int x, int y) const {
    TileType tile = getTile(x, y);
    return tile == TileType::FLOOR || 
           tile == TileType::PELLET || 
           tile == TileType::POWER ||
           tile == TileType::DOOR;
}

glm::vec3 Maze::getCenter() const {
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

TileType Maze::charToTile(char c) const {
    switch (c) {
        case '#': return TileType::WALL;
        case '.': return TileType::PELLET;
        case 'o': 
        case 'O': return TileType::POWER;
        case '-': return TileType::DOOR;
        case ' ': return TileType::EMPTY;
        default:  return TileType::FLOOR;
    }
}
