#include "DefaultCamera.h"
#include "shine.h"
#include <glm\gtc\matrix_transform.hpp>

CDefaultCamera::CDefaultCamera():
m_currentCamerMode(ICamera::EDITOR)
{
}

CDefaultCamera::~CDefaultCamera()
{

}

bool CDefaultCamera::Init()
{
	return true;
}

void CDefaultCamera::Update(float dt, GLFWwindow* pWin)
{
	// Simple camera system. 
	if (m_currentCamerMode == EDITOR)
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

	if (m_currentCamerMode == GAME)
	{
		m_cameraPos = Vec3(0, 2.0f, 20.0f);
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
	}
	// ---------------------------------------


	glm::mat4 ProjectionMatrix = glm::perspective(90.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	glm::mat4 ViewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_direction, glm::vec3(0, 1, 0));

	// Build the VP matrix
	m_vpm = ProjectionMatrix * ViewMatrix;
}

void CDefaultCamera::SetCameraMode(ICamera::CameraMode mode)
{
	m_currentCamerMode = mode;
}