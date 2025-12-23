#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Shader class for OpenGL shader program management.
 * Handles loading, compiling, and using vertex/fragment shaders.
 */
class Shader {
public:
    unsigned int program_id;
    
    Shader() : program_id(0) {}
    ~Shader();
    
    // Load and compile shaders from file paths
    bool load(const std::string& vertex_path, const std::string& fragment_path);
    
    // Activate shader program
    void use() const;
    
    // Uniform setters
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    
private:
    // Compile a single shader from source
    unsigned int compileShader(unsigned int type, const std::string& source);
    
    // Read file contents
    std::string readFile(const std::string& path);
    
    // Check compilation/linking errors
    bool checkCompileErrors(unsigned int shader, const std::string& type);
    bool checkLinkErrors(unsigned int program);
};

#endif // SHADER_H
