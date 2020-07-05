#include <iostream>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

//Window size
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader, uniformXMove;

bool bDirection = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

// vertex shader
static const char* vertexShader = "								\n\
#version 330													\n\
layout (location = 0) in vec3 pos;								\n\
uniform float xMove;											\n\
void main(){													\n\
	gl_Position = vec4(0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);	\n\
}";

// fragment shader
static const char* fragmentShader = "		\n\
#version 330								\n\
out vec4 colour;							\n\
void main(){								\n\
	colour = vec4(1.0, 0.0, 0.0, 1.0);		\n\
}";

void CreateTriangle()
{
	GLfloat Vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
	
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling %d shader: %s\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
	return;
}

void CompileShaders()
{
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Shader not created");
		return;
	}

	AddShader(shader, vertexShader, GL_VERTEX_SHADER);
	AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s\n", eLog);
		return;
	}

	uniformXMove = glGetUniformLocation(shader, "xMove");
	return;
}

int main()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		cout << "GLFW init failed\n";
		glfwTerminate();
		return 1;
	}

	//setup GLFW window properties
	//openGl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//Core profile = No backward compatibility
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* pMainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);
	if (!pMainWindow)
	{
		cout << "GLFW window failed\n";
		glfwTerminate();
		return 1;
	}
	//Get buffer size information
	int iBufferWidth = 0, iBufferHeight = 0;
	glfwGetFramebufferSize(pMainWindow, &iBufferWidth, &iBufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(pMainWindow);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW init failed\n";
		glfwDestroyWindow(pMainWindow);
		glfwTerminate();
		return 1;
	}

	//Setup viewport size
	glViewport(0, 0, iBufferWidth, iBufferHeight);

	CreateTriangle();
	CompileShaders();
	//Loop until window closed
	while (!glfwWindowShouldClose(pMainWindow))
	{
		//Get + handle user input events
		glfwPollEvents();

		if (bDirection)
			triOffset += triIncrement;
		else
			triOffset -= triIncrement;

		if (abs(triOffset) >= triMaxOffset)
			bDirection = !bDirection;
		//clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glUniform1f(uniformXMove, triOffset);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(pMainWindow);
	}

	return 0;
}