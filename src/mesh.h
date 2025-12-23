#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoord;
    
    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& col)
        : position(pos), normal(norm), color(col), texCoord(0.0f, 0.0f) {}
    
    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& col, const glm::vec2& uv)
        : position(pos), normal(norm), color(col), texCoord(uv) {}
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    
    void create(const std::vector<Vertex>& vertices);
    void draw() const;
    void bind() const;
    void unbind() const;
    size_t getVertexCount() const { return vertex_count; }
    
private:
    unsigned int vao;
    unsigned int vbo;
    size_t vertex_count;
};

Mesh createCube(const glm::vec3& color);
Mesh createBrickCube(const glm::vec3& baseColor);
Mesh createFloorTile(const glm::vec3& color);
Mesh createTexturedCube(const glm::vec3& color);
Mesh createTexturedFloorTile(const glm::vec3& color);

#endif // MESH_H
