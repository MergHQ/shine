#include "Camera.h"
#include "DefaultCamera.h"
#include <GLFW\glfw3.h>
#include "shine.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init()
{
	return true;
}

void CCamera::Update(float dt, GLFWwindow* pWin)
{
	gSys->GetDefaultCamera()->Update(dt, pWin); // Overriding not working TODO: Fix this!
}