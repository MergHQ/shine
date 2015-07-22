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
	virtual void SetCameraMode(CameraMode mode) {}
	virtual Mat44 GetVPMatrix() { return Mat44(); }
	virtual Mat44 GetViewMatrix() = 0;
	virtual Mat44 GetProjectionMatrix() = 0;
	virtual Vec3 GetWorldPos() = 0;
	//~ICamera

	virtual void SetCameraSpeed(float speed) {}
protected:
	virtual void setMovement(bool forward, bool backwards, bool right, bool left){}
private:
};
#endif