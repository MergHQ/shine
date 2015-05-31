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
	//~ICamera

	virtual void SetCameraSpeed(float speed) {}
	virtual void setMovement(bool forward, bool backwards, bool right, bool left){}
};


#endif