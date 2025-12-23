#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#include "stb_image.h"
#include "tiny_gltf.h"

#include "../glad/glad.h"
#include "model.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model() : loaded(false) {}

Model::~Model() {
    cleanup();
}

void Model::cleanup() {
    for (auto& mesh : meshes) {
        if (mesh.VAO) glDeleteVertexArrays(1, &mesh.VAO);
        if (mesh.VBO) glDeleteBuffers(1, &mesh.VBO);
        if (mesh.EBO) glDeleteBuffers(1, &mesh.EBO);
    }
    meshes.clear();
    loaded = false;
}

bool Model::load(const std::string& filepath) {
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF loader;
    std::string err, warn;
    
    bool ret = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, filepath);
    
    if (!warn.empty()) std::cout << "GLTF Warning: " << warn << std::endl;
    if (!err.empty()) std::cerr << "GLTF Error: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load GLB: " << filepath << std::endl;
        return false;
    }
    
    // Process all meshes
    for (size_t meshIdx = 0; meshIdx < gltfModel.meshes.size(); meshIdx++) {
        const auto& mesh = gltfModel.meshes[meshIdx];
        for (const auto& primitive : mesh.primitives) {
            std::vector<float> vertices;
            std::vector<unsigned int> indices;
            
            // Get material color
            glm::vec3 baseColor(1.0f);
            if (primitive.material >= 0) {
                const auto& material = gltfModel.materials[primitive.material];
                const auto& pbr = material.pbrMetallicRoughness;
                if (pbr.baseColorFactor.size() >= 3) {
                    baseColor.r = static_cast<float>(pbr.baseColorFactor[0]);
                    baseColor.g = static_cast<float>(pbr.baseColorFactor[1]);
                    baseColor.b = static_cast<float>(pbr.baseColorFactor[2]);
                }
            }
            
            // Get position accessor
            if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                const auto& buffer = gltfModel.buffers[bufferView.buffer];
                
                const float* positions = reinterpret_cast<const float*>(
                    &buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                
                // Get normals
                const float* normals = nullptr;
                if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                    const auto& normAccessor = gltfModel.accessors[primitive.attributes.at("NORMAL")];
                    const auto& normBufferView = gltfModel.bufferViews[normAccessor.bufferView];
                    normals = reinterpret_cast<const float*>(
                        &buffer.data[normBufferView.byteOffset + normAccessor.byteOffset]);
                }
                
                // Get vertex colors if available
                const float* vertColors = nullptr;
                if (primitive.attributes.find("COLOR_0") != primitive.attributes.end()) {
                    const auto& colAccessor = gltfModel.accessors[primitive.attributes.at("COLOR_0")];
                    const auto& colBufferView = gltfModel.bufferViews[colAccessor.bufferView];
                    vertColors = reinterpret_cast<const float*>(
                        &buffer.data[colBufferView.byteOffset + colAccessor.byteOffset]);
                }
                
                for (size_t i = 0; i < accessor.count; i++) {
                    // Position
                    vertices.push_back(positions[i * 3]);
                    vertices.push_back(positions[i * 3 + 1]);
                    vertices.push_back(positions[i * 3 + 2]);
                    // Normal
                    if (normals) {
                        vertices.push_back(normals[i * 3]);
                        vertices.push_back(normals[i * 3 + 1]);
                        vertices.push_back(normals[i * 3 + 2]);
                    } else {
                        vertices.push_back(0.0f);
                        vertices.push_back(1.0f);
                        vertices.push_back(0.0f);
                    }
                    // Color (from vertex colors or material)
                    if (vertColors) {
                        vertices.push_back(vertColors[i * 4]);
                        vertices.push_back(vertColors[i * 4 + 1]);
                        vertices.push_back(vertColors[i * 4 + 2]);
                    } else {
                        vertices.push_back(baseColor.r);
                        vertices.push_back(baseColor.g);
                        vertices.push_back(baseColor.b);
                    }
                }
            }
            
            // Get indices
            if (primitive.indices >= 0) {
                const auto& accessor = gltfModel.accessors[primitive.indices];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                const auto& buffer = gltfModel.buffers[bufferView.buffer];
                
                const unsigned char* data = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
                
                for (size_t i = 0; i < accessor.count; i++) {
                    unsigned int index = 0;
                    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                        index = reinterpret_cast<const unsigned short*>(data)[i];
                    } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                        index = reinterpret_cast<const unsigned int*>(data)[i];
                    } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                        index = data[i];
                    }
                    indices.push_back(index);
                }
            }
            
            if (!vertices.empty()) {
                processMesh(vertices, indices, baseColor);
            }
        }
    }
    
    loaded = !meshes.empty();
    if (loaded) {
        std::cout << "Loaded model: " << filepath << " (" << meshes.size() << " meshes)" << std::endl;
    }
    return loaded;
}

void Model::processMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, glm::vec3 color) {
    ModelMesh mesh;
    mesh.color = color;
    mesh.indexCount = indices.size();
    
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    
    glBindVertexArray(mesh.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    // Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    
    glBindVertexArray(0);
    
    meshes.push_back(mesh);
}

void Model::render(Shader& shader, const glm::mat4& modelMat, const glm::mat4& view, const glm::mat4& proj) {
    if (!loaded) return;
    
    shader.use();
    shader.setMat4("model", modelMat);
    shader.setMat4("view", view);
    shader.setMat4("projection", proj);
    
    for (const auto& mesh : meshes) {
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexCount), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
