#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::Render(GLFWwindow* pWin)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gSys->pMeshSystem != nullptr)
	{
		for (int iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			// Shader drawing
			glUseProgram(gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader()->GetShaderProgramme());
			gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader()->Update();

			// Mesh drawing
			glBindBuffer(GL_ARRAY_BUFFER, gSys->pMeshSystem->GetMeshContainer()[iter]->meshVbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gSys->pMeshSystem->GetMeshContainer()[iter]->meshInidcies);
			glDrawElements(GL_TRIANGLES, gSys->pMeshSystem->GetMeshContainer()[iter]->GetIndicies().size(), GL_UNSIGNED_INT, 0);


			// Simple camera system. 
			// TODO make a camera system.
			double xpos, ypos;

			glfwGetCursorPos(pWin, &xpos, &ypos);

			m_horizontalAngle += m_sensitivity * (m_oldx - xpos);
			m_verticalAngle += m_sensitivity * (m_oldy - ypos);

			m_oldx = xpos;
			m_oldy = ypos;

			glm::vec3 direction(
				cos(m_verticalAngle) * sin(m_horizontalAngle),
				sin(m_verticalAngle),
				cos(m_verticalAngle) * cos(m_horizontalAngle)
				);

			glm::vec3 right = glm::vec3(
				sin(m_horizontalAngle - 3.14f / 2.0f),
				0,
				cos(m_horizontalAngle - 3.14f / 2.0f)
				);

			glm::vec3 up = glm::cross(right, direction);

			// Move forward
			if (glfwGetKey(pWin, GLFW_KEY_W) == GLFW_PRESS)
			{
				m_cameraPos += direction * m_speed;
			}
			// Move backward
			if (glfwGetKey(pWin, GLFW_KEY_S) == GLFW_PRESS)
			{
				m_cameraPos -= direction * m_speed;
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

			glm::mat4 ProjectionMatrix = glm::perspective(90.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
			glm::mat4 ViewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + direction, glm::vec3(0, 1, 0));
			GLuint p = gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader()->GetShaderProgramme();

			glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix * ViewMatrix * gSys->pMeshSystem->GetMeshContainer()[iter]->GetWorldTM()));

		}
	}
}
