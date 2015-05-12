#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GL\GL.h>
#include <stdlib.h>
#include <stdio.h>
#include "shine.h"
#include "MainWindow.h"
#include "GlobalSystem.h"
#include <Windows.h>

IGlobalSystem* gSys;
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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
	window = glfwCreateWindow(1280, 720, "Shine - Render Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Creating the core systems
	gSys = new IGlobalSystem();
	gSys->Init();

	// Disable cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		exit(1);
	}

	std::vector<float> data2 = 
	{
		// Front face
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0,

		// Back face
		-1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,
		1.0, 1.0, -1.0,
		1.0, -1.0, -1.0,

		// Top face
		-1.0, 1.0, -1.0,
		-1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, -1.0,

		// Bottom face
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0, 1.0,
		-1.0, -1.0, 1.0,

		// Right face
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,
		1.0, 1.0, 1.0,
		1.0, -1.0, 1.0,

		// Left face
		-1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,
		-1.0, 1.0, -1.0
	};

	std::vector<float> data
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	SShaderParams sparams;
	sparams.id = 123;
	sparams.name = "sampleshader";
	sparams.f_file = "shaders/frag1.frag";
	sparams.v_file = "shaders/vertex1.vert";

	SShaderParams sparams2;
	sparams2.id = 12313;
	sparams2.name = "sampleshader2";
	sparams2.f_file = "shaders/frag2.frag";
	sparams2.v_file = "shaders/vertex1.vert";


	// Create a couple of sample meshes.
	SMeshParams mesh2;
	mesh2.name = "sample2";
	mesh2.fileName = "urmum.obj";
	mesh2.pos = Vec3(0.0f, 0.0f, 10.0f);
	mesh2.rotaxis = Vec3(0, 1, 0);
	mesh2.rotAmmount = 180 * 3.14 / 180.0f;
	mesh2.pShader = &sparams;
	IMesh* pMesh2 = gSys->pMeshSystem->CreateMesh(&mesh2);

	SMeshParams mesh3;
	mesh3.name = "sample2";
	mesh3.fileName = "test.obj";
	mesh3.pos = Vec3(0.0f, 0.0f, 5.0f);
	mesh3.pShader = &sparams2;
	IMesh* pMesh3 = gSys->pMeshSystem->CreateMesh(&mesh3);

	while (!glfwWindowShouldClose(window))
	{
		gSys->pRenderer->Render(window);
		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	if (CMeshSystem* pMeshSys = gSys->pMeshSystem)
	{
		for (unsigned int iter = 0; iter < pMeshSys->GetMeshContainer().size(); iter++)
		{
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshVao);
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshVbo);
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshInidcies);

			glDeleteProgram(pMeshSys->GetMeshContainer()[iter]->GetShader()->GetShaderProgramme());
		}
	}
	if (CMeshSystem* pMeshSys = gSys->pMeshSystem)
	{
		for (unsigned int iter = 0; iter < pMeshSys->GetMeshContainer().size(); iter++)
		{
			if (pMeshSys->GetMeshContainer()[iter] != nullptr)
			{
				// Delete all the meshes and shaders from memory, so we avoid memory leaks.
				delete pMeshSys->GetMeshContainer()[iter];
				delete pMeshSys->GetShaderConteainer()[iter];
			}
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

