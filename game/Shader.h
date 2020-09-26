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

enum class SHenum {
	SH_VERTEX,
	SH_FRAGMENT,
	SH_PROGRAM
};

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	
	void Use() const; // use/activate shader

	// compile status check functions
	void CheckCompile(GLuint id, SHenum type) const;

	// utility uniform functions
	GLint GetUniformLocation(const std::string& name) const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

	void SetMat2(const std::string& name, glm::mat2 value, GLboolean transpose) const;
	void SetMat3(const std::string& name, glm::mat3 value, GLboolean transpose) const;
	void SetMat4(const std::string& name, glm::mat4 value, GLboolean transpose) const;
	void SetVec2(const std::string& name, glm::vec2 value) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetVec4(const std::string& name, glm::vec4 value) const;
};

#endif // !SHADER_H

