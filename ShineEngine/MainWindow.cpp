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
	gSys->pLevelLoader->LoadLevel("assetzoo.shinedata");

	for (uint i = 0; i < 50; i++)
	{
		SMeshParams tree;
		tree.fileName = "objects/lptree.obj";
		tree.m_materialFile = "tree.mtl";
		tree.name = "dsad";
		if (rand() % 4 < 1)
			tree.pos = Vec3(rand() % 10, rand() % 10, rand() % 10);
		else if (rand() % 4 < 2)
			tree.pos = Vec3(rand() % 10, rand() % 10, rand() % 10) * Vec3(-1,1,1);
		else if (rand() % 4 < 3)
			tree.pos = Vec3(rand() % 10, rand() % 10, rand() % 10) * Vec3(1,1,-1);
		else 
			tree.pos = Vec3(rand() % -10, rand() % 10, rand() % -10) * Vec3(-1,1,-1);
		gSys->pMeshSystem->CreateMesh(&tree);

	}

	Light sun;
	sun.type = DIRLIGHT;
	sun.color = Vec3(4, 2, 0);
	sun.position = Vec3(700, 200, 700);
	ILight* pl = gSys->pRenderer->GetLightSystem()->CreateLight(&sun);

	// Disable cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load a scene

	// Set the camera mode
	gSys->GetCamera()->SetCameraMode(ICamera::EDITOR);

	SMeshParams p;
	p.fileName = "objects/sphere.obj";
	p.m_materialFile = "m.mtl";
	p.pos = Vec3(0, 0, 0);
	p.name = "dsads";
	//IMesh* pm = gSys->pMeshSystem->CreateMesh(&p);

	// Timer variables
	double lastTime = glfwGetTime();
	float dt = 0;
	float t = 0;

	while (!glfwWindowShouldClose(window))
	{
		// Update timer
		t += 0.08f;
		dt = float(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		gSys->pRenderer->Render();
		gSys->Update(dt * 60); // We run around 60 fps
		glfwPollEvents();
		glfwSwapBuffers(window);
		//pm->SetPos(Vec3(sin(t) * 10, sin(t) * 10, 0));

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

