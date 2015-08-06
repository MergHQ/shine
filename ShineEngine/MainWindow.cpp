#include "MainWindow.h"
#include "shine.h"
#include "FPCamera.h"
#include "Input.h"
#include "WindowInput.h"
#include "GameInput.h"
#include "MeshSystem.h"
#include "Renderer.h"
#include "MaterialSystem.h"
#include "EnvironmentLoader.h"

#include <GL\glew.h>
#include <GL\GL.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <random>

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
	gSys->Init(this);

	for (uint i = 0; i < 100; i++)
	{
		SMeshParams tree;
		tree.fileName = "objects/trees.obj";
		tree.m_materialFile = "tree.mtl";
		tree.name = "dsad";
		if (rand() % 4 < 1)
			tree.pos = Vec3(rand() % 100, 0, rand() % 100);
		else if (rand() % 4 < 2)
			tree.pos = Vec3(rand() % 100, 0, rand() % 100) * Vec3(-1,1,1);
		else if (rand() % 4 < 3)
			tree.pos = Vec3(rand() % 100, 0, rand() % 100) * Vec3(1,1,-1);
		else 
			tree.pos = Vec3(rand() % -100, 0, rand() % -100) * Vec3(-1);
		gSys->pMeshSystem->CreateMesh(&tree);

	}

	Light light;
	light.type = DIRLIGHT;
	light.color = Vec3(2, 2, 1);
	light.position = Vec3(300, 20, 300);
	//gSys->pRenderer->GetLightSystem()->CreateLight(&light);

	// Disable cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load a scene
	gSys->pLevelLoader->LoadLevel("assetzoo.shinedata");

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


	gSys->ReleaseRenderContent();

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

