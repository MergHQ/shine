#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"
#include "DefaultCamera.h"

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
	window = pWin;
	glEnable(GL_LIGHTING);
	glClearColor(1.0f, 1.0f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gSys->pMeshSystem != nullptr)
	{
		for (int iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			// Shader drawing
			GLuint p = gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader()->GetShaderProgramme();
			glUseProgram(p);
			gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader()->Update();

			// Mesh drawing
			// Verticies
			glBindBuffer(GL_ARRAY_BUFFER, gSys->pMeshSystem->GetMeshContainer()[iter]->meshVbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			// Indicies
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gSys->pMeshSystem->GetMeshContainer()[iter]->GetIbo());

			// Normals
			glBindBuffer(GL_ARRAY_BUFFER, gSys->pMeshSystem->GetMeshContainer()[iter]->GetNbo());
			glNormalPointer(GL_FLOAT, gSys->pMeshSystem->GetMeshContainer()[iter]->GetVerts().size(), BUFFER_OFFSET(12));

			glDrawElements(GL_TRIANGLES, gSys->pMeshSystem->GetMeshContainer()[iter]->GetIndicies().size(), GL_UNSIGNED_INT, 0);

			// You da real model view matrix*1
			glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetDefaultCamera()->GetVPMatrix() * gSys->pMeshSystem->GetMeshContainer()[iter]->GetWorldTM()));

		}
	}
}
