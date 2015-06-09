#ifndef FPCAMERA_H
#define FPCAMERA_H

#pragma once

#include "Camera.h"
#include "IInputListener.h"

class CFPCamera : public CCamera, public IInputListener
{
public:
	CFPCamera();
	~CFPCamera();

	// CCamera
	virtual bool Init();
	virtual void Update(float dt, GLFWwindow* pWin) override;
	virtual glm::mat4 GetVPMatrix() { return m_pmatrix * m_vmatrix; }
	virtual void SetCameraMode(CameraMode mode) { m_currentCameraMode = mode; }
	virtual void SetCameraSpeed(float speed) { m_speed = speed; }
	virtual void setMovement(bool forward, bool backwards, bool right, bool left) override;
	//~CCamera

	//IInputListener
	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//~IInputListener

	void move(GLFWwindow *window, float delta);

private:
	double m_horizontalAngle = 0, m_verticalAngle = 0;
	float m_speed = 0.7f;
	float m_sensitivity = 0.001f;
	int m_currentCameraMode;
	double xpos, ypos;
	int width, height;
	double m_oldx = 0, m_oldy = 0;
	bool m_forward = false, m_backwards = false, m_right = false, m_left = false;
	glm::mat4 m_vmatrix;
	glm::mat4 m_pmatrix;
	glm::vec3 m_direction;
	glm::vec3 m_cameraPos;
};

#endif