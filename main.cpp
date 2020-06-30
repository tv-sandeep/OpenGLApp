#include <iostream>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

//Window size
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

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

	//Loop until window closed
	while (!glfwWindowShouldClose(pMainWindow))
	{
		//Get + handle user input events
		glfwPollEvents();

		//Clear window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(pMainWindow);
	}
	return 0;
}