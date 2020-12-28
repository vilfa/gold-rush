#include "Renderer/Shader.h"

Shader::Shader(const std::string _vertex_path,
	const std::string _fragment_path)
{
	std::string vertex_source, fragment_source;
	std::ifstream v_shader_file, f_shader_file;
	std::stringstream v_shader_stream, f_shader_stream;

	// Make sure ifstream objects can throw exceptions.
	// Specify the state flags for which a failure exception is thrown
	// (failbit = logical error, badbit = read/writing error).
	//
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		v_shader_file.open(_vertex_path);
		f_shader_file.open(_fragment_path);

		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();

		v_shader_file.close();
		f_shader_file.close();

		vertex_source = v_shader_stream.str();
		fragment_source = f_shader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::SHADER::FILE_READ_ERROR" << std::endl;
		std::cout << "Error:" << e.what() << std::endl;
	}

	const char* v_shader_source = vertex_source.c_str();
	const char* f_shader_source = fragment_source.c_str();

	// Create vertex and fragment shaders.
	//
	uint32_t vertex_shader, fragment_shader;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &v_shader_source, NULL);
	glCompileShader(vertex_shader);
	CheckCompile(vertex_shader, SHTYPEenum::VERTEX);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &f_shader_source, NULL);
	glCompileShader(fragment_shader);
	CheckCompile(fragment_shader, SHTYPEenum::FRAGMENT);

	// Create shader program and link.
	//
	id_ = glCreateProgram();
	glAttachShader(id_, vertex_shader);
	glAttachShader(id_, fragment_shader);
	glLinkProgram(id_);
	CheckCompile(id_, SHTYPEenum::PROGRAM);

	// Delete shaders because link is successful.
	//
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::Shader(const std::string _vertex_path,
	const std::string _geometry_path,
	const std::string _fragment_path)
{
	std::string vertex_source, geometry_source, fragment_source;
	std::ifstream v_shader_file, g_shader_file, f_shader_file;
	std::stringstream v_shader_stream, g_shader_stream, f_shader_stream;

	// Make sure ifstream objects can throw exceptions.
	// Specify the state flags for which a failure exception is thrown
	// (failbit = logical error, badbit = read/writing error).
	//
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		v_shader_file.open(_vertex_path);
		g_shader_file.open(_geometry_path);
		f_shader_file.open(_fragment_path);

		v_shader_stream << v_shader_file.rdbuf();
		g_shader_stream << g_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();

		v_shader_file.close();
		g_shader_file.close();
		f_shader_file.close();

		vertex_source = v_shader_stream.str();
		geometry_source = g_shader_stream.str();
		fragment_source = f_shader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::SHADER::FILE_READ_ERROR" << std::endl;
		std::cout << "Error:" << e.what() << std::endl;
	}

	const char* v_shader_source = vertex_source.c_str();
	const char* g_shader_source = geometry_source.c_str();
	const char* f_shader_source = fragment_source.c_str();

	// Create vertex, geometry and fragment shaders.
	//
	uint32_t vertex_shader, geometry_shader, fragment_shader;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &v_shader_source, NULL);
	glCompileShader(vertex_shader);
	CheckCompile(vertex_shader, SHTYPEenum::VERTEX);

	geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader, 1, &g_shader_source, NULL);
	glCompileShader(geometry_shader);
	CheckCompile(geometry_shader, SHTYPEenum::GEOMETRY);
	
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &f_shader_source, NULL);
	glCompileShader(fragment_shader);
	CheckCompile(fragment_shader, SHTYPEenum::FRAGMENT);

	// Create shader program and link.
	//
	id_ = glCreateProgram();
	glAttachShader(id_, vertex_shader);
	glAttachShader(id_, geometry_shader);
	glAttachShader(id_, fragment_shader);
	glLinkProgram(id_);
	CheckCompile(id_, SHTYPEenum::PROGRAM);

	// Delete shaders because link is successful.
	//
	glDeleteShader(vertex_shader);
	glDeleteShader(geometry_shader);
	glDeleteShader(fragment_shader);
}

void Shader::Use() const
{
	glUseProgram(id_);
}

void Shader::CheckCompile(GLuint id, const SHTYPEenum _type) const
{
	int success;
	char info_log[512];

	if (_type == SHTYPEenum::VERTEX || _type == SHTYPEenum::GEOMETRY || _type == SHTYPEenum::FRAGMENT)
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::CHECK_COMPILE::COMPILATION_FAILED" << std::endl;
			std::cout << "Type:" << (int) _type << "|Log:" << std::endl << info_log << std::endl;
		}
	}
	else if (_type == SHTYPEenum::PROGRAM)
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::CHECK_COMPILE::LINK_FAILED" << std::endl;
			std::cout << info_log << std::endl;
		}
	}
}

GLint Shader::GetUniformLocation(const std::string _name) const
{
	GLint location = glGetUniformLocation(id_, _name.c_str());
	if (location < 0)
	{
		std::cout << "ERROR::SHADER::GET_UNIFORM_LOCATION::UNIFORM_DOESNT_EXIST" << std::endl;
		std::cout << "Uniform name:" << _name << std::endl;
	}

	return location;
}

void Shader::SetBool(const std::string _name, const bool _value) const
{
	glUniform1i(GetUniformLocation(_name), (int)_value);
}

void Shader::SetInt(const std::string _name, const int _value) const
{
	glUniform1i(GetUniformLocation(_name), _value);
}

void Shader::SetFloat(const std::string _name, const float _value) const
{
	glUniform1f(GetUniformLocation(_name), _value);
}

void Shader::SetMat2(const std::string _name, const glm::mat2& _value, GLboolean transpose) const
{
	glUniformMatrix2fv(GetUniformLocation(_name), 1, transpose, glm::value_ptr(_value));
}

void Shader::SetMat3(const std::string _name, const glm::mat3& _value, GLboolean transpose) const
{
	glUniformMatrix3fv(GetUniformLocation(_name), 1, transpose, glm::value_ptr(_value));
}

void Shader::SetMat4(const std::string _name, const glm::mat4& _value, GLboolean transpose) const
{
	glUniformMatrix4fv(GetUniformLocation(_name), 1, transpose, glm::value_ptr(_value));
}

void Shader::SetVec2(const std::string _name, const glm::vec2& _value) const
{
	glUniform2fv(GetUniformLocation(_name), 1, glm::value_ptr(_value));
}

void Shader::SetVec3(const std::string _name, const glm::vec3& _value) const
{
	glUniform3fv(GetUniformLocation(_name), 1, glm::value_ptr(_value));
}

void Shader::SetVec4(const std::string _name, const glm::vec4& _value) const
{
	glUniform4fv(GetUniformLocation(_name), 1, glm::value_ptr(_value));
}
