#ifndef ICAMERA_H
#define ICAMERA_H
#pragma once

#include "shine.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

struct ICamera
{ 
	enum CameraMode
	{
		EDITOR,
		GAME
	};

	virtual ~ICamera(){}
	virtual bool Init() = 0;
	// Called every frame.
	virtual void Update(float dt, GLFWwindow* pWin) = 0;
	virtual void SetCameraMode(ICamera::CameraMode mode) = 0;
	virtual Vec3 GetWorldPos() = 0;
	virtual Mat44 GetViewMatrix() = 0;
	virtual Mat44 GetProjectionMatrix() = 0;
	virtual Mat44 GetVPMatrix() = 0;


};
#endif