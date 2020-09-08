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

enum SHenum {
	SH_VERTEX = 0,
	SH_FRAGMENT = 1,
	SH_PROGRAM = 2,
};

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath, SHenum type);
	
	void use() const; // use/activate shader

	// compile status check functions
	void checkCompile(GLuint id, SHenum type) const;

	// utility uniform functions
	GLint getUniformLocation(const std::string& name) const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value, GLboolean transpose) const;
};

#endif // !SHADER_H

