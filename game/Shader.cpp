#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, SHenum type)
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
		std::cout << "ERROR::SHADER::FILE_READ_ERROR" << std::endl;
	}

	const char* vShaderSource = vertexSource.c_str();
	const char* fShaderSource = fragmentSource.c_str();

	/*
	* Create vertex and fragment shader.
	*/
	unsigned int vertexShader, fragmentShader;
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderSource, NULL);
	glCompileShader(vertexShader);
	CheckCompile(vertexShader, SH_VERTEX);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
	glCompileShader(fragmentShader);
	CheckCompile(fragmentShader, SH_FRAGMENT);

	/*
	* Create shader program and link.
	*/
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	CheckCompile(ID, SH_PROGRAM);

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

void Shader::CheckCompile(GLenum id, SHenum type) const
{
	int success;
	char infoLog[512];

	switch (type)
	{
	case SH_VERTEX:
	case SH_FRAGMENT:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		break;
	case SH_PROGRAM:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		break;
	default:
		break;
	}

	if (!success)
	{
		switch (type)
		{
		case SH_VERTEX:
		case SH_FRAGMENT:
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED" << infoLog << std::endl;
			break;
		case SH_PROGRAM:
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED" << infoLog << std::endl;
			break;
		default:
			break;
		}
	}
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	if (location < 0)
	{
		std::cout << "ERROR::SHADER::UNIFORM::UNIFORM_DOESNT_EXIST"
			<< std::endl << "Uniform name:" << name << std::endl;
	}

	return location;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value, GLboolean transpose) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, glm::value_ptr(value));
}
