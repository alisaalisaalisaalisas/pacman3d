#ifndef RENDERER_H
#define RENDERER_H

#include "maze.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include <memory>

class MazeRenderer {
public:
    MazeRenderer();
    
    void buildFromMaze(const Maze& maze);
    void loadTextures();
    void render(Shader& shader, const Camera& camera);
    void renderPellets(Shader& shader, const Maze& maze);
    
private:
    std::unique_ptr<Mesh> wallMesh;
    std::unique_ptr<Mesh> floorMesh;
    std::unique_ptr<Mesh> pelletMesh;
    std::unique_ptr<Mesh> powerMesh;
    
    Texture wallTexture;
    Texture floorTexture;
    Texture cornerTexture;
    bool texturesLoaded = false;
    
    std::vector<glm::vec3> wallPositions;
    std::vector<glm::vec3> floorPositions;
    
    const Maze* mazeRef;
    
    static constexpr glm::vec3 WALL_COLOR{1.0f, 1.0f, 1.0f};
    static constexpr glm::vec3 FLOOR_COLOR{1.0f, 1.0f, 1.0f};
    static constexpr glm::vec3 PELLET_COLOR{1.0f, 0.9f, 0.2f};  // Yellow
    static constexpr glm::vec3 POWER_COLOR{1.0f, 0.85f, 0.0f};  // Bright yellow
};

#endif // RENDERER_H
