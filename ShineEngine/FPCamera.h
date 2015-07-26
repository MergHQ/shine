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

	// Operator overloads for using _aligned_malloc alignment 16 and _aligned_free
	void* operator new(size_t size);
	void operator delete(void* p);

	// CCamera
	virtual bool Init();
	virtual void Update(float dt, GLFWwindow* pWin) override;
	virtual void SetCameraMode(CameraMode mode) { m_currentCameraMode = mode; }
	virtual void SetCameraSpeed(float speed) { m_speed = speed; }
	virtual Vec3 GetWorldPos() { return m_cameraPos; }
	virtual glm::mat4 GetVPMatrix() { return m_pmatrix * m_vmatrix; }
	virtual glm::mat4 GetViewMatrix() { return m_vmatrix; }
	virtual glm::mat4 GetProjectionMatrix() { return m_pmatrix; }
	virtual int textures() { return texturess; }
	//~CCamera

	int texturess = 1;

	//IInputListener
	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//~IInputListener

	void move(GLFWwindow *window, float delta);
protected:
	virtual void setMovement(bool forward, bool backwards, bool right, bool left) override;
private:
	bool m_forward = false, m_backwards = false, m_right = false, m_left = false;
	double m_horizontalAngle = 0, m_verticalAngle = 0;
	double xpos, ypos;
	double m_oldx = 0, m_oldy = 0;
	float m_speed = 0.1f;
	float m_sensitivity = 0.001f;
	int m_currentCameraMode;
	int width, height;
	Vec3 m_direction;
	Vec3 m_cameraPos;
	glm::mat4 m_vmatrix;
	glm::mat4 m_pmatrix;
};
#endif