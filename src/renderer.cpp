#include "renderer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

MazeRenderer::MazeRenderer() : mazeRef(nullptr) {}

void MazeRenderer::buildFromMaze(const Maze& maze) {
    mazeRef = &maze;
    wallPositions.clear();
    floorPositions.clear();
    
    wallMesh = std::make_unique<Mesh>(createBrickCube(WALL_COLOR));
    floorMesh = std::make_unique<Mesh>(createFloorTile(FLOOR_COLOR));
    pelletMesh = std::make_unique<Mesh>(createCube(PELLET_COLOR));
    powerMesh = std::make_unique<Mesh>(createCube(POWER_COLOR));
    
    for (int y = 0; y < maze.getHeight(); ++y) {
        for (int x = 0; x < maze.getWidth(); ++x) {
            glm::vec3 worldPos = maze.gridToWorld(x, y);
            TileType tile = maze.getTile(x, y);
            
            switch (tile) {
                case TileType::WALL:
                    wallPositions.push_back(worldPos + glm::vec3(0.0f, 0.5f, 0.0f));
                    break;
                case TileType::FLOOR:
                case TileType::PELLET:
                case TileType::POWER:
                case TileType::DOOR:
                    floorPositions.push_back(worldPos);
                    break;
                case TileType::EMPTY:
                default:
                    break;
            }
        }
    }
    
    std::cout << "Built maze: " << wallPositions.size() << " walls, " 
              << floorPositions.size() << " floors" << std::endl;
}

void MazeRenderer::render(Shader& shader, const Camera& camera) {
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", camera.getProjectionMatrix());
    
    for (const auto& pos : wallPositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        shader.setMat4("model", model);
        wallMesh->draw();
    }
    
    for (const auto& pos : floorPositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        shader.setMat4("model", model);
        floorMesh->draw();
    }
}

void MazeRenderer::renderPellets(Shader& shader, const Maze& maze) {
    for (int y = 0; y < maze.getHeight(); ++y) {
        for (int x = 0; x < maze.getWidth(); ++x) {
            TileType tile = maze.getTile(x, y);
            glm::vec3 worldPos = maze.gridToWorld(x, y);
            
            if (tile == TileType::PELLET) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos + glm::vec3(0.0f, 0.2f, 0.0f));
                model = glm::scale(model, glm::vec3(0.15f));
                shader.setMat4("model", model);
                pelletMesh->draw();
            }
            else if (tile == TileType::POWER) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos + glm::vec3(0.0f, 0.3f, 0.0f));
                model = glm::scale(model, glm::vec3(0.35f));
                shader.setMat4("model", model);
                powerMesh->draw();
            }
        }
    }
}
