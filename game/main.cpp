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

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
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


	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnTheGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	/*
	* Adjust viewport size when window size is changed
	*/
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	/*
	Set initial viewport size
	*/
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	/*----- GEOMETRY -----*/
	/*
	* create VAO to store different VBOs
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/*
	* Create triangle vertices and bind the Vertex Buffer Object (VBO)
	*/
	float vertices[] = {
		 0.5f, 0.5f, 0.0f, // top r
		 0.5f, -0.5f, 0.0f, // bottom r
		-0.5f, -0.5f, 0.0f, // bottom l
		-0.5f, 0.5f, 0.0f, // top l
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO); // generate buffer at address of VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer to GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // fill currently bound buffer with data

	/*
	* create element buffer object to easily reuse vertices
	*/
	unsigned int indices[] = {
		0, 1, 3, // triangle 1
		1, 2, 3, // triangle 2
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
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
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompile(&fragmentShader, "FRAGMENT");

	/*
	* create shader program to link and use vertex and fragment shader 
	* (connect vertex outputs to fragment inputs, fragment to...)
	*/
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkShaderProgramLink(&shaderProgram);
	
	/*
	* delete shader after linking them as we don't need them any longer
	*/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*----- RENDER -----*/
	/* 
	* RENDER LOOP (ONE ITER. = FRAME) - Execute until GLFW is told to stop 
	* -glfwWindowShouldClose checks at start of each loop	if GLFW instructed close
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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
