#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include "ICamera.h"

class CCamera : public ICamera
{
public:
	CCamera();
	~CCamera();

	//ICamera
	virtual bool Init();
	virtual void Update(GLFWwindow* pWin);
	virtual glm::mat4 GetVPMatrix() { return glm::mat4(); }
	virtual void SetCameraMode(CameraMode mode) {}
	//~ICamera

private:

};


#endif