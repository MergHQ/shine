#include "FPCamera.h"
#include "shine.h"
#include "Input.h"
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <iostream>


CFPCamera::CFPCamera()
{
	m_cameraPos = glm::vec3(0, 1.8f, 0);

	// Projection matrix : FOV = 45°, aspect ratio, near plane, far plane
	m_pmatrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
}

CFPCamera::~CFPCamera()
{

}

void* CFPCamera::operator new(size_t size)
{
	void *storage = _aligned_malloc(size, 16);
	if (NULL == storage)
	{
		throw "allocation fail : no free memory";
	}
	return storage;
}

void CFPCamera::operator delete(void* p)
{
	_aligned_free(p);
}

bool CFPCamera::Init()
{
	gSys->pInput->addListener(this);
	return true;
}

void CFPCamera::Update(float dt, GLFWwindow* pWin)
{
	// Looking around
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

	// Movement
	if (m_forward != m_backwards || m_right != m_left)
		move(pWin, dt);

	// Construct View Matrix
	m_vmatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_direction, glm::vec3(0, 1, 0));
}

bool CFPCamera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool move =
		glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

	setMovement(
		glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS, glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS,
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS, glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS
		);
	return move;
}


void CFPCamera::setMovement(bool forward, bool backwards, bool right, bool left)
{
	m_forward = forward;
	m_backwards = backwards;
	m_right = right;
	m_left = left;
}

void CFPCamera::move(GLFWwindow *window, float delta)
{
	glm::vec3 right = glm::vec3(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
		);

	glm::vec3 up = glm::cross(right, m_direction);

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

	bool editor = m_currentCameraMode == EDITOR;

	if (m_forward)
	{
		m_cameraPos += (editor ? m_direction : forwardDir) * delta * m_speed;
	}
	if (m_backwards)
	{
		m_cameraPos -= (editor ? m_direction : forwardDir) * delta * m_speed;
	}

	if (m_right)
	{
		m_cameraPos += (editor ? right : rightDir) * delta * m_speed;
	}
	if (m_left)
	{
		m_cameraPos -= (editor ? right : rightDir) * delta * m_speed;
	}
}