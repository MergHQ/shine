#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include "ICamera.h"
enum CameraMode;

class CCamera : public ICamera
{
public:
	CCamera();
	~CCamera();

	//ICamera
	virtual bool Init();
	virtual void Update(float dt, GLFWwindow* pWin);
	virtual glm::mat4 GetVPMatrix() { return glm::mat4(); }
	virtual void SetCameraMode(CameraMode mode) {}
	virtual glm::vec3 GetWorldPos() = 0;
	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;
	//~ICamera

	virtual void SetCameraSpeed(float speed) {}
	virtual void setMovement(bool forward, bool backwards, bool right, bool left){}
};


#endif