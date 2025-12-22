#include "shader.h"
#include "../glad/glad.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::~Shader() {
    if (program_id != 0) {
        glDeleteProgram(program_id);
    }
}

bool Shader::load(const std::string& vertex_path, const std::string& fragment_path) {
    // Read shader source files
    std::string vertex_source = readFile(vertex_path);
    std::string fragment_source = readFile(fragment_path);
    
    if (vertex_source.empty() || fragment_source.empty()) {
        std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
        return false;
    }
    
    // Compile shaders
    unsigned int vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_source);
    if (vertex_shader == 0) return false;
    
    unsigned int fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragment_source);
    if (fragment_shader == 0) {
        glDeleteShader(vertex_shader);
        return false;
    }
    
    // Create and link program
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    
    bool success = checkLinkErrors(program_id);
    
    // Cleanup shaders (linked into program now)
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return success;
}

void Shader::use() const {
    glUseProgram(program_id);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(program_id, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    std::string type_name = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
    if (!checkCompileErrors(shader, type_name)) {
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER: Could not open file: " << path << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << info_log << std::endl;
        return false;
    }
    return true;
}

bool Shader::checkLinkErrors(unsigned int program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        return false;
    }
    return true;
}
