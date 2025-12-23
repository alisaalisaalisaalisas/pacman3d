#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"

struct ModelMesh {
    unsigned int VAO, VBO, EBO;
    size_t indexCount;
    glm::vec3 color;
};

class Model {
public:
    Model();
    ~Model();
    
    bool load(const std::string& filepath);
    void render(Shader& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj);
    void cleanup();
    
    bool isLoaded() const { return loaded; }
    
private:
    std::vector<ModelMesh> meshes;
    bool loaded;
    
    void processMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, glm::vec3 color);
};

#endif // MODEL_H
