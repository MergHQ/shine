#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GL\GL.h>
#include <stdlib.h>
#include <stdio.h>
#include "shine.h"
#include "MainWindow.h"
#include "GlobalSystem.h"


IGlobalSystem* gSys;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
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

	if (glewInit() != GLEW_OK)
	{
		exit(1);
	}

	std::vector<float> data2 = 
	{
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

	float data3[] =
	{
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

	SShaderParams sparams;
	sparams.id = 1231;
	sparams.name = "sampleshader";
	sparams.f_file = "shaders/frag1.frag";
	sparams.v_file = "shaders/vertex1.vert";


	SMeshParams mesh;

	mesh.name = "sample";
	mesh.id = 123;
	mesh.verts = data2;
	//mesh.pos = Vec3(0, 0, 0);
	mesh.pShader = &sparams;

	IMesh* pMesh = gSys->pMeshSystem->CreateMesh(&mesh);
	while (!glfwWindowShouldClose(window))
	{
		gSys->pRenderer->Render();
		glfwPollEvents();
		glfwSwapBuffers(window);		
	}

	if (CMeshSystem* pMeshSys = gSys->pMeshSystem)
	{
		for (unsigned int iter = 0; iter < pMeshSys->GetMeshContainer().size(); iter++)
		{
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshVao);
			glDeleteShader(pMeshSys->GetShaderConteainer()[iter]->GetShaderProgramme());
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

