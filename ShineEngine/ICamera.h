#ifndef ICAMERA_H
#define ICAMERA_H

#pragma once

#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>

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
	virtual glm::mat4 GetVPMatrix() = 0;
	virtual void SetCameraMode(ICamera::CameraMode mode) = 0;
	virtual glm::vec3 GetWorldPos() = 0;
	virtual glm::mat4 GetViewMatrix() = 0;

};

#endif