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
	gSys->Init(this);

	gSys->pRenderer->Init(window);

	// Disable cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load a scene
	gSys->pLevelLoader->LoadLevel("assetzoo.shinedata");

	// Creating a couple lights
	//Light l1;
	//l1.attenuation = Vec3(1, 0.01f, 0.002f);
	//l1.position = Vec3(0, 0, 3);
	//l1.color = Vec3(1, 1, 1);
	//l1.type = POINTLIGHT;
	//ILight* pLight = gSys->pRenderer->GetLightSystem()->CreateLight(&l1);

	//Light l2;
	//l2.attenuation = Vec3(1, 0.01f, 0.002f);
	//l2.position = Vec3(0, 5, 50);
	//l2.color = Vec3(0, 1, 0);
	//l2.type = POINTLIGHT;
	//gSys->pRenderer->GetLightSystem()->CreateLight(&l2);


	//Light l3;
	//l3.attenuation = Vec3(1, 1, 1);
	//l3.position = Vec3(30, 5, 50);
	//l3.color = Vec3(0, 0, 1);
	//l3.type = POINTLIGHT;
	//gSys->pRenderer->GetLightSystem()->CreateLight(&l3);
	//---

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

