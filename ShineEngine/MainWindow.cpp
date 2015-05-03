#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GL\GL.h>
#include <stdlib.h>
#include <stdio.h>
#include "shine.h"
#include "MainWindow.h"

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

int main(void)
{
	CMainWindow mw;
	mw.Init();
}

void CMainWindow::Init()
{ 
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1280, 720, "Shine - RenderWindow", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Creating the core systems
	IGlobalSystem* gSys = new IGlobalSystem();
	gSys->Init();

	if (glewInit() != GLEW_OK)
	{
		exit(1);
	}

	float data2[] = 
	{
		0.0f, 0.1f, 1.0f,
		1.1f, -0.1f, 1.0f,
		-0.1f, -1.1f, 1.0f
	};

	float data3[] =
	{
		1.0f, 0.1f, 1.0f,
		1.1f, -0.1f, 1.0f,
		-1.1f, -0.1f, 1.0f
	};

	gSys->pRenderer->CreateMesh(data2, sizeof(data2), "shaders/vertex1.vert", "shaders/frag1.frag");
	gSys->pRenderer->CreateMesh(data2, sizeof(data2), "shaders/vertex1.vert", "shaders/frag1.frag");
	gSys->pRenderer->CreateMesh(data3, sizeof(data3), "shaders/vertex1.vert", "shaders/frag1.frag");

	while (!glfwWindowShouldClose(window))
	{
		gSys->pRenderer->Render();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	if (CRenderer* pMc = gSys->pRenderer)
	{
		for (int iter = 0; iter < gSys->pRenderer->GetVaos().size(); iter++)
		{
			glDeleteBuffers(1, &gSys->pRenderer->GetVaos()[iter]);
			glDeleteShader(gSys->pRenderer->GetShaders()[iter]);
		}
	}
	glfwDestroyWindow(window);
	delete gSys;
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

CMainWindow::CMainWindow()
{
}

CMainWindow::~CMainWindow()
{
}

