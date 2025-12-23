#include "mesh.h"
#include "../glad/glad.h"

Mesh::Mesh() : vao(0), vbo(0), vertex_count(0) {}

Mesh::~Mesh() {
    if (vbo != 0) glDeleteBuffers(1, &vbo);
    if (vao != 0) glDeleteVertexArrays(1, &vao);
}

Mesh::Mesh(Mesh&& other) noexcept 
    : vao(other.vao), vbo(other.vbo), vertex_count(other.vertex_count) {
    other.vao = 0;
    other.vbo = 0;
    other.vertex_count = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (vbo != 0) glDeleteBuffers(1, &vbo);
        if (vao != 0) glDeleteVertexArrays(1, &vao);
        vao = other.vao;
        vbo = other.vbo;
        vertex_count = other.vertex_count;
        other.vao = 0;
        other.vbo = 0;
        other.vertex_count = 0;
    }
    return *this;
}

void Mesh::create(const std::vector<Vertex>& vertices) {
    vertex_count = vertices.size();
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // TexCoord
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertex_count));
    glBindVertexArray(0);
}

void Mesh::bind() const { glBindVertexArray(vao); }
void Mesh::unbind() const { glBindVertexArray(0); }

Mesh createCube(const glm::vec3& color) {
    std::vector<Vertex> vertices;
    
    const glm::vec3 front(0, 0, 1), back(0, 0, -1);
    const glm::vec3 left(-1, 0, 0), right(1, 0, 0);
    const glm::vec3 top(0, 1, 0), bottom(0, -1, 0);
    const float h = 0.5f;
    
    // Front face
    vertices.emplace_back(glm::vec3(-h, -h, h), front, color);
    vertices.emplace_back(glm::vec3(h, -h, h), front, color);
    vertices.emplace_back(glm::vec3(h, h, h), front, color);
    vertices.emplace_back(glm::vec3(-h, -h, h), front, color);
    vertices.emplace_back(glm::vec3(h, h, h), front, color);
    vertices.emplace_back(glm::vec3(-h, h, h), front, color);
    
    // Back face
    vertices.emplace_back(glm::vec3(h, -h, -h), back, color);
    vertices.emplace_back(glm::vec3(-h, -h, -h), back, color);
    vertices.emplace_back(glm::vec3(-h, h, -h), back, color);
    vertices.emplace_back(glm::vec3(h, -h, -h), back, color);
    vertices.emplace_back(glm::vec3(-h, h, -h), back, color);
    vertices.emplace_back(glm::vec3(h, h, -h), back, color);
    
    // Left face
    vertices.emplace_back(glm::vec3(-h, -h, -h), left, color);
    vertices.emplace_back(glm::vec3(-h, -h, h), left, color);
    vertices.emplace_back(glm::vec3(-h, h, h), left, color);
    vertices.emplace_back(glm::vec3(-h, -h, -h), left, color);
    vertices.emplace_back(glm::vec3(-h, h, h), left, color);
    vertices.emplace_back(glm::vec3(-h, h, -h), left, color);
    
    // Right face
    vertices.emplace_back(glm::vec3(h, -h, h), right, color);
    vertices.emplace_back(glm::vec3(h, -h, -h), right, color);
    vertices.emplace_back(glm::vec3(h, h, -h), right, color);
    vertices.emplace_back(glm::vec3(h, -h, h), right, color);
    vertices.emplace_back(glm::vec3(h, h, -h), right, color);
    vertices.emplace_back(glm::vec3(h, h, h), right, color);
    
    // Top face
    vertices.emplace_back(glm::vec3(-h, h, h), top, color);
    vertices.emplace_back(glm::vec3(h, h, h), top, color);
    vertices.emplace_back(glm::vec3(h, h, -h), top, color);
    vertices.emplace_back(glm::vec3(-h, h, h), top, color);
    vertices.emplace_back(glm::vec3(h, h, -h), top, color);
    vertices.emplace_back(glm::vec3(-h, h, -h), top, color);
    
    // Bottom face
    vertices.emplace_back(glm::vec3(-h, -h, -h), bottom, color);
    vertices.emplace_back(glm::vec3(h, -h, -h), bottom, color);
    vertices.emplace_back(glm::vec3(h, -h, h), bottom, color);
    vertices.emplace_back(glm::vec3(-h, -h, -h), bottom, color);
    vertices.emplace_back(glm::vec3(h, -h, h), bottom, color);
    vertices.emplace_back(glm::vec3(-h, -h, h), bottom, color);
    
    Mesh mesh;
    mesh.create(vertices);
    return mesh;
}

// Brick-style cube with darker edges for 3D depth
Mesh createBrickCube(const glm::vec3& baseColor) {
    std::vector<Vertex> vertices;
    
    glm::vec3 topColor = baseColor;
    glm::vec3 sideColor = baseColor * 0.85f;
    glm::vec3 bottomColor = baseColor * 0.7f;
    
    const glm::vec3 front(0, 0, 1), back(0, 0, -1);
    const glm::vec3 left(-1, 0, 0), right(1, 0, 0);
    const glm::vec3 top(0, 1, 0), bottom(0, -1, 0);
    const float h = 0.48f;
    
    // Front face (side color)
    vertices.emplace_back(glm::vec3(-h, -h, h), front, sideColor);
    vertices.emplace_back(glm::vec3(h, -h, h), front, sideColor);
    vertices.emplace_back(glm::vec3(h, h, h), front, sideColor);
    vertices.emplace_back(glm::vec3(-h, -h, h), front, sideColor);
    vertices.emplace_back(glm::vec3(h, h, h), front, sideColor);
    vertices.emplace_back(glm::vec3(-h, h, h), front, sideColor);
    
    // Back face
    vertices.emplace_back(glm::vec3(h, -h, -h), back, sideColor);
    vertices.emplace_back(glm::vec3(-h, -h, -h), back, sideColor);
    vertices.emplace_back(glm::vec3(-h, h, -h), back, sideColor);
    vertices.emplace_back(glm::vec3(h, -h, -h), back, sideColor);
    vertices.emplace_back(glm::vec3(-h, h, -h), back, sideColor);
    vertices.emplace_back(glm::vec3(h, h, -h), back, sideColor);
    
    // Left face
    vertices.emplace_back(glm::vec3(-h, -h, -h), left, sideColor);
    vertices.emplace_back(glm::vec3(-h, -h, h), left, sideColor);
    vertices.emplace_back(glm::vec3(-h, h, h), left, sideColor);
    vertices.emplace_back(glm::vec3(-h, -h, -h), left, sideColor);
    vertices.emplace_back(glm::vec3(-h, h, h), left, sideColor);
    vertices.emplace_back(glm::vec3(-h, h, -h), left, sideColor);
    
    // Right face
    vertices.emplace_back(glm::vec3(h, -h, h), right, sideColor);
    vertices.emplace_back(glm::vec3(h, -h, -h), right, sideColor);
    vertices.emplace_back(glm::vec3(h, h, -h), right, sideColor);
    vertices.emplace_back(glm::vec3(h, -h, h), right, sideColor);
    vertices.emplace_back(glm::vec3(h, h, -h), right, sideColor);
    vertices.emplace_back(glm::vec3(h, h, h), right, sideColor);
    
    // Top face (brightest)
    vertices.emplace_back(glm::vec3(-h, h, h), top, topColor);
    vertices.emplace_back(glm::vec3(h, h, h), top, topColor);
    vertices.emplace_back(glm::vec3(h, h, -h), top, topColor);
    vertices.emplace_back(glm::vec3(-h, h, h), top, topColor);
    vertices.emplace_back(glm::vec3(h, h, -h), top, topColor);
    vertices.emplace_back(glm::vec3(-h, h, -h), top, topColor);
    
    // Bottom face (darkest)
    vertices.emplace_back(glm::vec3(-h, -h, -h), bottom, bottomColor);
    vertices.emplace_back(glm::vec3(h, -h, -h), bottom, bottomColor);
    vertices.emplace_back(glm::vec3(h, -h, h), bottom, bottomColor);
    vertices.emplace_back(glm::vec3(-h, -h, -h), bottom, bottomColor);
    vertices.emplace_back(glm::vec3(h, -h, h), bottom, bottomColor);
    vertices.emplace_back(glm::vec3(-h, -h, h), bottom, bottomColor);
    
    Mesh mesh;
    mesh.create(vertices);
    return mesh;
}

Mesh createFloorTile(const glm::vec3& color) {
    std::vector<Vertex> vertices;
    const glm::vec3 up(0, 1, 0);
    const float h = 0.5f;
    const float y = 0.01f;
    
    vertices.emplace_back(glm::vec3(-h, y, -h), up, color);
    vertices.emplace_back(glm::vec3(h, y, -h), up, color);
    vertices.emplace_back(glm::vec3(h, y, h), up, color);
    vertices.emplace_back(glm::vec3(-h, y, -h), up, color);
    vertices.emplace_back(glm::vec3(h, y, h), up, color);
    vertices.emplace_back(glm::vec3(-h, y, h), up, color);
    
    Mesh mesh;
    mesh.create(vertices);
    return mesh;
}

Mesh createTexturedCube(const glm::vec3& color) {
    std::vector<Vertex> vertices;
    
    const glm::vec3 front(0, 0, 1), back(0, 0, -1);
    const glm::vec3 left(-1, 0, 0), right(1, 0, 0);
    const glm::vec3 top(0, 1, 0), bottom(0, -1, 0);
    const float h = 0.48f;
    
    // Front face with UV
    vertices.emplace_back(glm::vec3(-h, -h, h), front, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, -h, h), front, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(h, h, h), front, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, -h, h), front, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, h, h), front, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, h, h), front, color, glm::vec2(0, 1));
    
    // Back face
    vertices.emplace_back(glm::vec3(h, -h, -h), back, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(-h, -h, -h), back, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(-h, h, -h), back, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(h, -h, -h), back, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(-h, h, -h), back, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(h, h, -h), back, color, glm::vec2(0, 1));
    
    // Left face
    vertices.emplace_back(glm::vec3(-h, -h, -h), left, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(-h, -h, h), left, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(-h, h, h), left, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, -h, -h), left, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(-h, h, h), left, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, h, -h), left, color, glm::vec2(0, 1));
    
    // Right face
    vertices.emplace_back(glm::vec3(h, -h, h), right, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, -h, -h), right, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(h, h, -h), right, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(h, -h, h), right, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, h, -h), right, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(h, h, h), right, color, glm::vec2(0, 1));
    
    // Top face
    vertices.emplace_back(glm::vec3(-h, h, h), top, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, h, h), top, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(h, h, -h), top, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, h, h), top, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, h, -h), top, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, h, -h), top, color, glm::vec2(0, 1));
    
    // Bottom face
    vertices.emplace_back(glm::vec3(-h, -h, -h), bottom, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, -h, -h), bottom, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(h, -h, h), bottom, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, -h, -h), bottom, color, glm::vec2(0, 0));
    vertices.emplace_back(glm::vec3(h, -h, h), bottom, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(-h, -h, h), bottom, color, glm::vec2(0, 1));
    
    Mesh mesh;
    mesh.create(vertices);
    return mesh;
}

Mesh createTexturedFloorTile(const glm::vec3& color) {
    std::vector<Vertex> vertices;
    const glm::vec3 up(0, 1, 0);
    const float h = 0.5f;
    const float y = 0.01f;
    
    // Counter-clockwise winding when viewed from above (correct for OpenGL CCW front face)
    vertices.emplace_back(glm::vec3(-h, y, h), up, color, glm::vec2(0, 1));
    vertices.emplace_back(glm::vec3(h, y, h), up, color, glm::vec2(1, 1));
    vertices.emplace_back(glm::vec3(h, y, -h), up, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(-h, y, h), up, color, glm::vec2(0, 1));
    vertices.emplace_back(glm::vec3(h, y, -h), up, color, glm::vec2(1, 0));
    vertices.emplace_back(glm::vec3(-h, y, -h), up, color, glm::vec2(0, 0));
    
    Mesh mesh;
    mesh.create(vertices);
    return mesh;
}
