#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"
#include "Camera.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CRenderer::CRenderer():
time(0)
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::Init()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void CRenderer::Render(GLFWwindow* pWin)
{
	window = pWin;
	glEnable(GL_LIGHTING);
	glClearColor(1.0f, 1.0f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLuint p = 0;
	time += 0.01f;
	if (gSys->pMeshSystem != nullptr)
	{
		for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
			{
				// Shader drawing
				if (pMesh->GetShader())
					p = pMesh->GetShader()->GetShaderProgramme();
				glUseProgram(p);
				pMesh->GetShader()->Update();

				glBindVertexArray(pMesh->GetVao());

				glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));

				// Textures
				// Create one OpenGL texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pMesh->GetTextureId());
				glUniform1i(glGetUniformLocation(p, "texsamp"), 0);
				glUniform1f(glGetUniformLocation(p, "shp"), sin(time));

				glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);

				//glUniform1f(glGetUniformLocation(p, "shp"), sin(time));

				glBindVertexArray(NULL);
			}
		}
	}
}
