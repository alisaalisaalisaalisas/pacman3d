#include "renderer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

MazeRenderer::MazeRenderer() : mazeRef(nullptr) {}

void MazeRenderer::loadTextures() {
    if (wallTexture.load("assets/sprites/pacman-sprite-wall-1766447227855.png")) {
        std::cout << "Loaded wall texture" << std::endl;
    }
    if (floorTexture.load("assets/sprites/pacman-sprite-ground-1766445952654.png")) {
        std::cout << "Loaded floor texture" << std::endl;
    }
    if (cornerTexture.load("assets/sprites/pacman-sprite-corner-1766447240710.png")) {
        std::cout << "Loaded corner texture" << std::endl;
    }
    texturesLoaded = true;
}

void MazeRenderer::buildFromMaze(const Maze& maze) {
    mazeRef = &maze;
    wallPositions.clear();
    floorPositions.clear();
    
    // Use textured meshes
    wallMesh = std::make_unique<Mesh>(createTexturedCube(WALL_COLOR));
    floorMesh = std::make_unique<Mesh>(createTexturedFloorTile(FLOOR_COLOR));
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
    
    // Render walls with texture
    if (texturesLoaded && wallTexture.getID() != 0) {
        shader.setBool("useTexture", true);
        wallTexture.bind(0);
        shader.setInt("textureSampler", 0);
    } else {
        shader.setBool("useTexture", false);
    }
    
    for (const auto& pos : wallPositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        shader.setMat4("model", model);
        wallMesh->draw();
    }
    
    // Unbind wall texture
    wallTexture.unbind();
    
    // Render floors with texture
    if (texturesLoaded && floorTexture.getID() != 0) {
        shader.setBool("useTexture", true);
        floorTexture.bind(0);
        shader.setInt("textureSampler", 0);
    } else {
        shader.setBool("useTexture", false);
    }
    
    for (const auto& pos : floorPositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        shader.setMat4("model", model);
        floorMesh->draw();
    }
    
    // Unbind floor texture
    floorTexture.unbind();
    // Reset texture state
    shader.setBool("useTexture", false);
}

void MazeRenderer::renderPellets(Shader& shader, const Maze& maze) {
    shader.setBool("useTexture", false);
    
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
