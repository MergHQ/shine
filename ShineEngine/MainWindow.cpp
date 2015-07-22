#include "MainWindow.h"
#include "shine.h"
#include "FPCamera.h"
#include "Input.h"
#include "WindowInput.h"
#include "GameInput.h"
#include "MeshSystem.h"
#include "Renderer.h"
#include "MaterialSystem.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GL\GL.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

IGlobalSystem* gSys;
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	gSys->pInput->key_callback(window, key, scancode, action, mods);
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
	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK)
	{
		exit(1);
	}

	// Creating the core systems
	gSys = new IGlobalSystem();
	gSys->Init();

	gSys->pRenderer->Init(window);

	// Disable cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	SMeshParams mesh2;
	mesh2.name = "sample1";
	mesh2.fileName = "objects/sponza.obj";
	mesh2.pos = Vec3(0.0f, 0.0f, 5.0f);
	mesh2.m_materialFile = "m.mtl";
	IMesh* pMesh2 = gSys->pMeshSystem->CreateMesh(&mesh2);

	// Set the camera mode
	gSys->GetCamera()->SetCameraMode(ICamera::EDITOR);

	// Timer variables
	double lastTime = glfwGetTime();
	float dt = 0;
	while (!glfwWindowShouldClose(window))
	{
		// Update timer
		dt = float(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		gSys->pRenderer->Render();
		gSys->Update(dt * 60); // We run around 60 fps
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	Release();

	glfwDestroyWindow(window);
	gSys->Log("Bye.");
	delete gSys;
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void CMainWindow::Release()
{
	if (CMeshSystem* pMeshSys = gSys->pMeshSystem)
	{
		for (unsigned int iter = 0; iter < pMeshSys->GetMeshContainer().size(); iter++)
		{
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshVbo);
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshInidcies);
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshNormals);
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshTexcoords);
			//glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshTextureId);

			glDeleteProgram(pMeshSys->GetMeshContainer()[iter]->GetMaterial()->GetShader()->GetShaderProgramme());
		}
	}

	if (CMeshSystem* pMeshSys = gSys->pMeshSystem)
	{
		gSys->Log("Purging shaders, meshes, textures and materials.");
		for (uint iter = 0; iter < pMeshSys->GetMeshContainer().size(); iter++)
		{
			if (pMeshSys->GetMeshContainer()[iter] != nullptr)
			{
				// Delete all the meshes and shaders from memory, so we avoid memory leaks.
				delete pMeshSys->GetMeshContainer()[iter];
			}
		}

		for (uint j = 0; j < gSys->pMaterialSystem->GetMaterialContainer().size(); j++)
			delete gSys->pMaterialSystem->GetMaterialContainer()[j];
	}

	gSys->pInput->clearListeners();
}


CMainWindow::CMainWindow()
{
}

CMainWindow::~CMainWindow()
{
}

