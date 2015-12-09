#include "MainWindow.h"
#include "shine.h"
#include "FPCamera.h"
#include "Input.h"
#include "WindowInput.h"
#include "GameInput.h"
#include "MeshSystem.h"
#include "Renderer.h"
#include "MaterialSystem.h"
#include "EnvironmentIO.h"

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

	if (key == GLFW_KEY_I && action == GLFW_RELEASE)
	{
		Light cl;
		cl.attenuation = Vec3(1, 1, 1);
		cl.color = Vec3(5);
		cl.position = gSys->GetCamera()->GetWorldPos();
		cl.type = POINTLIGHT;
		gSys->pRenderer->GetLightSystem()->CreateLight(&cl);
	}

	if (key == GLFW_KEY_M && action == GLFW_RELEASE)
	{
		SMeshParams tree;
		tree.fileName = "objects/witcher3.obj";
		tree.m_materialFile = "witcher3.mtl";
		tree.name = "dsad";
		tree.pos = gSys->GetCamera()->GetWorldPos();
		gSys->pMeshSystem->CreateMesh(&tree);
	}

	if (key == GLFW_KEY_N && action == GLFW_RELEASE)
	{
		SMeshParams tree;
		tree.fileName = "objects/small_plane.obj";
		tree.m_materialFile = "plane.mtl";
		tree.name = "dsad";
		tree.pos = gSys->GetCamera()->GetWorldPos();
		gSys->pMeshSystem->CreateMesh(&tree);
	}
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

	for (uint i = 0; i <10; i++)
	{

		for (uint j = 0; j < 10; j++)
		{
			SMeshParams tree;
			tree.fileName = "objects/untitled.obj";
			tree.m_materialFile = "materials/barrel.mtl";
			tree.name = "dsad";
			tree.pos = Vec3(i*3,0,j*3);
			gSys->pMeshSystem->CreateMesh(&tree);
		}
	}

	// Disable cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load a scene
	//gSys->pLevelLoader->LoadLevel("sexdungeon.shinedata");
	glfwWindowHint(GLFW_SRGB_CAPABLE, TRUE);

	// Set the camera mode
	gSys->GetCamera()->SetCameraMode(ICamera::EDITOR);

	// Timer variables
	double lastTime = glfwGetTime();
	float dt = 0;
	float t = 0;
	glfwSwapInterval(0);

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

