#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class SHTYPEenum
{
	VERTEX,
	FRAGMENT,
	PROGRAM
};

class Shader
{
public:
	unsigned int ID;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	
	void Use() const; // use/activate shader

	// compile status check functions
	void CheckCompile(GLuint& id, SHTYPEenum type) const;

	// utility uniform functions
	GLint GetUniformLocation(const std::string& name) const;

	void SetBool(const std::string& name, const bool& value) const;
	void SetInt(const std::string& name, const int& value) const;
	void SetFloat(const std::string& name, const float& value) const;

	void SetMat2(const std::string& name, const glm::mat2& value, GLboolean transpose = GL_FALSE) const;
	void SetMat3(const std::string& name, const glm::mat3& value, GLboolean transpose = GL_FALSE) const;
	void SetMat4(const std::string& name, const glm::mat4& value, GLboolean transpose = GL_FALSE) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
};

#endif // !SHADER_H

