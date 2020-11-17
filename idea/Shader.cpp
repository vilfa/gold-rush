#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexSource, fragmentSource;
	std::ifstream vShaderFile, fShaderFile;
	std::stringstream vShaderStream, fShaderStream;

	/*
	* Make sure ifstream objects can throw exceptions.
	* Specify the state flags for which a failure exception is thrown
	* (failbit = logical error, badbit = read/writing error).
	*/
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	/*
	* Read shader files.
	*/
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexSource = vShaderStream.str();
		fragmentSource = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::SHADER::FILE_READ_ERROR" << std::endl;
		std::cout << "Error:" << e.what() << std::endl;
	}

	const char* vShaderSource = vertexSource.c_str();
	const char* fShaderSource = fragmentSource.c_str();

	/*
	* Create vertex and fragment shader.
	*/
	uint32_t vertexShader, fragmentShader;
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderSource, NULL);
	glCompileShader(vertexShader);
	CheckCompile(vertexShader, SHTYPEenum::VERTEX);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
	glCompileShader(fragmentShader);
	CheckCompile(fragmentShader, SHTYPEenum::FRAGMENT);

	/*
	* Create shader program and link.
	*/
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	CheckCompile(ID, SHTYPEenum::PROGRAM);

	/*
	* Delete shader because link is successful.
	*/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

void Shader::CheckCompile(GLuint& id, SHTYPEenum type) const
{
	int success;
	char infoLog[512];

	if (type == SHTYPEenum::VERTEX || type == SHTYPEenum::FRAGMENT)
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::CHECK_COMPILE::COMPILATION_FAILED" << std::endl;
			std::cout << "Type:" << (int)type << "|Log:" << infoLog << std::endl;
		}
	}
	else if (type == SHTYPEenum::PROGRAM)
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::CHECK_COMPILE::LINK_FAILED" << std::endl;
			std::cout << infoLog << std::endl;
		}
	}
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	if (location < 0)
	{
		std::cout << "ERROR::SHADER::GET_UNIFORM_LOCATION::UNIFORM_DOESNT_EXIST" << std::endl;
		std::cout << "Uniform name:" << name << std::endl;
	}

	return location;
}

void Shader::SetBool(const std::string& name, const bool& value) const
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, const int& value) const
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, const float& value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value, GLboolean transpose) const
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, transpose, glm::value_ptr(value));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value, GLboolean transpose) const
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, transpose, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value, GLboolean transpose) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, glm::value_ptr(value));
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}
