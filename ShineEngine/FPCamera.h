#ifndef FPCAMERA_H
#define FPCAMERA_H

#pragma once

#include "Camera.h"

class CFPCamera : public CCamera
{
public:
	CFPCamera();
	~CFPCamera();

	// CCamera
	virtual bool Init();
	virtual void Update(float dt, GLFWwindow* pWin) override;
	virtual glm::mat4 GetVPMatrix() { return m_vpmatrix; }
	virtual void SetCameraMode(CameraMode mode) {}

private:
	double m_horizontalAngle = 0, m_verticalAngle = 0;
	float m_speed = 0.1f;
	float m_sensitivity = 0.001f;
	glm::mat4 m_vpmatrix;
	glm::vec3 m_direction;
	glm::vec3 m_cameraPos;
};

#endif