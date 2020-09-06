#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void checkShaderCompile(GLuint*, const char*);
void checkShaderProgramLink(GLuint*);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderOrangeSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
"}\n";

const char* fragmentShaderYellowSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.75f, 0.6f, 0.2f, 1.0f);"
"}\n";

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	/*
	* create window
	*/
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnTheGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::INIT::GLFW::WINDOW\n" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::INIT::GLAD\n" << std::endl;
		glfwTerminate();
		return -1;
	}

	/*
	* set viewport size change callback and initial viewport size
	*/
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	/*----- GEOMETRY -----*/
	/*
	* create VAO to store different VBOs, EBO to reuse vertices
	* you can bind all different kinds of buffers to the VAO!
	*/
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	/*
	* create triangle vertices and bind the Vertex Buffer Object (VBO)
	*/
	float vertices[] = {
		 0.5f, 0.5f, 0.0f,	// top r
		 0.5f, -0.5f, 0.0f, // bottom r
		-0.5f, -0.5f, 0.0f, // bottom l
		-0.5f, 0.5f, 0.0f,	// top l
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	* create element buffer object (EBO) to easily reuse vertices
	*/
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	* tell theGL how to interperet the vertex data VBO
	* (each value is 4B/32b float)
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		

	/*----- SHADERS -----*/
	/*
	* create, compile, check compile of vertex shader
	*/
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkShaderCompile(&vertexShader, "VERTEX");

	/*
	* create, compile, check compile of fragment shader
	*/
	unsigned int fragmentShaders[2];
	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaders[0], 1, &fragmentShaderOrangeSource, NULL);
	glShaderSource(fragmentShaders[1], 1, &fragmentShaderYellowSource, NULL);
	glCompileShader(fragmentShaders[0]);
	glCompileShader(fragmentShaders[1]);
	checkShaderCompile(&fragmentShaders[0], "FRAGMENT");
	checkShaderCompile(&fragmentShaders[1], "FRAGMENT");

	/*
	* create shader program to link and use vertex and fragment shader 
	* (connect vertex outputs to fragment inputs, fragment to...)
	*/
	unsigned int shaderPrograms[2];
	shaderPrograms[0] = glCreateProgram();
	shaderPrograms[1] = glCreateProgram();
	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);
	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);
	glLinkProgram(shaderPrograms[0]);
	glLinkProgram(shaderPrograms[1]);
	checkShaderProgramLink(&shaderPrograms[0]);
	checkShaderProgramLink(&shaderPrograms[1]);

	/*
	* delete shader after linking them as we don't need them any longer
	*/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);

	/*----- RENDER -----*/
	/* 
	* RENDER LOOP (ONE ITER. = FRAME) - Execute until GLFW is told to stop 
	* -glfwWindowShouldClose checks at start of each loop if GLFW instructed close
	* -glfwPollEvents checks for events (input,...), updates window state, calls registered callbacks
	* -glfwSwapBuffers swaps the front and back buffer (where next frame is rendered to)
	*/
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode (delete or use GL_FILL for normal)
		glUseProgram(shaderPrograms[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderPrograms[0]);
	glDeleteProgram(shaderPrograms[1]);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void checkShaderCompile(GLuint* vertexShader, const char* type)
{
	int success;
	char infoLog[512];
	glGetShaderiv(*vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(*vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << *type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void checkShaderProgramLink(GLuint* shaderProgram)
{
	int success;
	char infoLog[512];
	glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}
