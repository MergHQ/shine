#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GL\GL.h>
#include <stdlib.h>
#include <stdio.h>
#include "shine.h"
#include "MainWindow.h"
#include "GlobalSystem.h"
#include <Windows.h>
#include "FPCamera.h"
#include <iostream>
#include "Input.h"
#include "WindowInput.h"
#include "GameInput.h"
#include "MeshSystem.h"
#include "Renderer.h"
#include "MaterialSystem.h"

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

	// Creating the core systems
	gSys = new IGlobalSystem();
	gSys->Init();

	// Disable cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		exit(1);
	}

	SShaderParams sparams;
	sparams.id = 123;
	sparams.name = "sampleshader";
	sparams.f_file = "shaders/texshader.frag";
	sparams.v_file = "shaders/texshader.vert";

	SMeshParams mesh3;
	mesh3.name = "sample2";
	mesh3.fileName = "objects/test.obj";
	mesh3.textureFile = "t.bmp";
	mesh3.pos = Vec3(0.0f, 0.0f, 5.0f);
	mesh3.pShader = &sparams;
	IMesh* pMesh3 = gSys->pMeshSystem->CreateMesh(&mesh3);

	SMeshParams mesh;
	mesh.name = "sample1";
	mesh.fileName = "objects/well.obj";
	mesh.textureFile = "t.bmp";
	mesh.pos = Vec3(10.0f, 3.0f, 150.0f);
	mesh.pShader = &sparams;
	IMesh* pMesh = gSys->pMeshSystem->CreateMesh(&mesh);

	gSys->pMaterialSystem->LoadMaterial("m.mtl");

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

		gSys->pRenderer->Render(window);
		gSys->Update(dt * 60); // We run around 60 fps
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
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshNormals);
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshTexcoords);
			glDeleteBuffers(1, &pMeshSys->GetMeshContainer()[iter]->meshTextureId);

			glDeleteProgram(pMeshSys->GetMeshContainer()[iter]->GetShader()->GetShaderProgramme());
		}
	}
	if (CMeshSystem* pMeshSys = gSys->pMeshSystem)
	{
		gSys->Log("Purging shaders and meshes.");
		for (uint iter = 0; iter < pMeshSys->GetMeshContainer().size(); iter++)
		{
			if (pMeshSys->GetMeshContainer()[iter] != nullptr)
			{
				// Delete all the meshes and shaders from memory, so we avoid memory leaks.
				delete pMeshSys->GetMeshContainer()[iter];
				delete pMeshSys->GetShaderConteainer()[iter];
			}
		}

		for (uint j = 0; j < gSys->pMaterialSystem->GetMaterialContainer().size(); j++)
			delete gSys->pMaterialSystem->GetMaterialContainer()[j];
	}

	gSys->pInput->clearListeners();
	glfwDestroyWindow(window);
	gSys->Log("Bye.");
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

