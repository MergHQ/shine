#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

class CRenderer
{

public:
	CRenderer();
	~CRenderer();
	void Render(GLFWwindow* pWin);
private:
	double m_horizontalAngle = 0, m_verticalAngle = 0;
	float m_speed = 0.1f;
	float m_sensitivity = 0.001f;
	double m_oldx = 0, m_oldy = 0;

	glm::vec3 m_cameraPos;
};

#endif