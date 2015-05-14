#include "Camera.h"
#include "DefaultCamera.h"
#include <GLFW\glfw3.h>

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

void CCamera::Update(GLFWwindow* pWin)
{
	gSys->GetDefaultCamera()->Update(pWin);
}