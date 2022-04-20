#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Types/EShader.h"

class Shader
{
public:
    uint32_t id_;

    Shader(const std::string _vertex_path, const std::string _fragment_path);
    Shader(const std::string _vertex_path,
           const std::string _geometry_path,
           const std::string _fragment_path);

    void Use() const;
    void CheckCompile(GLuint id, const SHTYPEenum _type) const;
    GLint GetUniformLocation(const std::string _name) const;
    void SetBool(const std::string _name, const bool _value) const;
    void SetInt(const std::string _name, const int _value) const;
    void SetFloat(const std::string _name, const float _value) const;
    void
    SetMat2(const std::string _name, const glm::mat2 &_value, GLboolean transpose = GL_FALSE) const;
    void
    SetMat3(const std::string _name, const glm::mat3 &_value, GLboolean transpose = GL_FALSE) const;
    void
    SetMat4(const std::string _name, const glm::mat4 &_value, GLboolean transpose = GL_FALSE) const;
    void SetVec2(const std::string _name, const glm::vec2 &_value) const;
    void SetVec3(const std::string _name, const glm::vec3 &_value) const;
    void SetVec4(const std::string _name, const glm::vec4 &_value) const;
};
