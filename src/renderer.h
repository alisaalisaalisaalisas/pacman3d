#ifndef RENDERER_H
#define RENDERER_H

#include "maze.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include <memory>

class MazeRenderer {
public:
    MazeRenderer();
    
    void buildFromMaze(const Maze& maze);
    void render(Shader& shader, const Camera& camera);
    void renderPellets(Shader& shader, const Maze& maze);
    
private:
    std::unique_ptr<Mesh> wallMesh;
    std::unique_ptr<Mesh> floorMesh;
    std::unique_ptr<Mesh> pelletMesh;
    std::unique_ptr<Mesh> powerMesh;
    
    std::vector<glm::vec3> wallPositions;
    std::vector<glm::vec3> floorPositions;
    
    const Maze* mazeRef;
    
    static constexpr glm::vec3 WALL_COLOR{0.95f, 0.92f, 0.88f};
    static constexpr glm::vec3 FLOOR_COLOR{0.75f, 0.75f, 0.72f};
    static constexpr glm::vec3 PELLET_COLOR{1.0f, 1.0f, 0.8f};
    static constexpr glm::vec3 POWER_COLOR{1.0f, 0.8f, 0.4f};
};

#endif // RENDERER_H
