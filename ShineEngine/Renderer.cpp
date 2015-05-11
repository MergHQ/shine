#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"

#include <GL\glew.h>
#include <vector>
#include <fstream>
#include <iostream>

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::Render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gSys->pMeshSystem != nullptr)
	{
		for (unsigned int iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			//glm::vec4 meshWorldPos = glm::vec4(gSys->pMeshSystem->GetMeshContainer()[iter]->GetWorldPos(), 1);
			//glm::mat4 meshMatrix;
			//glm::vec4 transformation = meshMatrix * meshWorldPos;
			//glUniformMatrix4fv(transformation);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();

			// Shader drawing
			glUseProgram(gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader()->GetShaderProgramme());
			gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader()->Update();
			
			// Mesh drawing
			glBindVertexArray(gSys->pMeshSystem->GetMeshContainer()[iter]->meshVao);
			glDrawArrays(GL_TRIANGLES, 0, 16);
		}
	}
}
