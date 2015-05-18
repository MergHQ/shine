#ifndef DEFAULTCAMERA_H
#define DEFAULTCAMERA_H

#pragma once

#include "Camera.h"

class CDefaultCamera : public CCamera
{
public:
	CDefaultCamera();
	~CDefaultCamera();

	//CCamera
	virtual bool Init() override;
	virtual void Update(float dt, GLFWwindow* pWin) override;
	virtual glm::mat4 GetVPMatrix() { return m_vpm; }
	virtual void SetCameraMode(ICamera::CameraMode mode);
	//~CCamera

private:
	double m_horizontalAngle = 0, m_verticalAngle = 0;
	float m_speed = 0.1f;
	float m_sensitivity = 0.001f;
	double m_oldx = 0, m_oldy = 0;
	int m_currentCamerMode;
	glm::mat4 m_vpm;
	glm::vec3 m_direction;
	glm::vec3 m_cameraPos;
};



#endif