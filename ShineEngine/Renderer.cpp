#include "Renderer.h"
#include "MainWindow.h"
#include "MeshSystem.h"
#include "Mesh.h"

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
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION_MATRIX);
	//gluLookAt
	//	(0.0, 0.0, 30.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0);
	if (gSys->pMeshSystem != nullptr)
	{
		for (unsigned int iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glUseProgram(gSys->pMeshSystem->GetMeshContainer()[iter]->GetShader());
			glBindVertexArray(gSys->pMeshSystem->GetMeshContainer()[iter]->meshVao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
}
