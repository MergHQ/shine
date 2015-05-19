#include "FPCamera.h"
#include "shine.h"
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <iostream>

CFPCamera::CFPCamera()
{
	m_cameraPos = glm::vec3(0, 1.8f, 0);
}

CFPCamera::~CFPCamera()
{

}

bool CFPCamera::Init()
{
	return true;
}

void CFPCamera::Update(float dt, GLFWwindow* pWin)
{
	float delta = dt * 60;		// We run around 60 fps
	if (m_currentCameraMode == GAME)
	{
		glfwGetCursorPos(pWin, &xpos, &ypos);			// Get cursor position
		glfwGetWindowSize(pWin, &width, &height);		// Get window size
		glfwSetCursorPos(pWin, width / 2, height / 2);	// Set cursor position at centre

		m_horizontalAngle += m_sensitivity * delta * (width / 2.0f - xpos);
		m_verticalAngle += m_sensitivity * delta * (height / 2.0f - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		m_direction = glm::vec3(
			cos(m_verticalAngle) * sin(m_horizontalAngle),
			sin(m_verticalAngle),
			cos(m_verticalAngle) * cos(m_horizontalAngle)
			);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(m_horizontalAngle - 3.14f / 2.0f),
			0,
			cos(m_horizontalAngle - 3.14f / 2.0f)
			);

		// Forward on ground
		glm::vec3 forwardDir = glm::vec3(
			m_direction.x,
			0,
			m_direction.z
			);

		// Right on ground
		glm::vec3 rightDir = glm::vec3(
			-forwardDir.z,
			0,
			forwardDir.x
			);

		// Up vector : perpendicular to both direction and right
		glm::vec3 up = glm::cross(right, m_direction);

		// Move forward
		if (glfwGetKey(pWin, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_cameraPos += forwardDir * delta * m_speed;
		}
		// Move backward
		if (glfwGetKey(pWin, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_cameraPos -= forwardDir * delta * m_speed;
		}
		// Strafe right
		if (glfwGetKey(pWin, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_cameraPos += rightDir * delta * m_speed;
		}
		// Strafe left
		if (glfwGetKey(pWin, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_cameraPos -= rightDir * delta * m_speed;
		}
	}

	if (m_currentCameraMode == EDITOR)
	{
		double xpos, ypos;

		glfwGetCursorPos(pWin, &xpos, &ypos);

		m_horizontalAngle += m_sensitivity * (m_oldx - xpos);
		m_verticalAngle += m_sensitivity * (m_oldy - ypos);

		m_oldx = xpos;
		m_oldy = ypos;

		m_direction = glm::vec3(
			cos(m_verticalAngle) * sin(m_horizontalAngle),
			sin(m_verticalAngle),
			cos(m_verticalAngle) * cos(m_horizontalAngle)
			);

		glm::vec3 right = glm::vec3(
			sin(m_horizontalAngle - 3.14f / 2.0f),
			0,
			cos(m_horizontalAngle - 3.14f / 2.0f)
			);

		glm::vec3 up = glm::cross(right, m_direction);

		// Move forward
		if (glfwGetKey(pWin, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_cameraPos += m_direction * m_speed;
		}
		// Move backward
		if (glfwGetKey(pWin, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_cameraPos -= m_direction * m_speed;
		}
		// Strafe right
		if (glfwGetKey(pWin, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_cameraPos += right  * m_speed;
		}
		// Strafe left
		if (glfwGetKey(pWin, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_cameraPos -= right * m_speed;
		}
	}

	// Projection matrix : FOV = 45°, aspect ratio, near plane, far plane
	glm::mat4 ProjectionMatrix = glm::perspective(45.0f, (float) width / (float) height, 0.1f, 1000.0f);
	glm::mat4 ViewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_direction, glm::vec3(0, 1, 0));

	// Construct View Projection Matrix
	m_vpmatrix = ProjectionMatrix * ViewMatrix;
}