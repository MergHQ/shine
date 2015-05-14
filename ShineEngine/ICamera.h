#ifndef ICAMERA_H
#define ICAMERA_H

#pragma once

#include "shine.h"

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
	virtual void Update(GLFWwindow* pWin) = 0;
	virtual glm::mat4 GetVPMatrix() = 0;
	virtual void SetCameraMode(ICamera::CameraMode mode) = 0;

};

#endif