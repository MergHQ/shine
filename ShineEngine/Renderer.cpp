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

				// Mesh drawing
				// Verticies
				glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVbo());
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

				// Indicies
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetIbo());

				// Normals
				glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetNbo());
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

				// Texcoords
				glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetTbo());
				glTexCoordPointer(pMesh->GetTexCoords().size(), GL_FLOAT, 0, 0);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL); // Pass UVs to the shader.

				// Textures
				// Create one OpenGL texture
				glEnable(GL_TEXTURE);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pMesh->GetTextureId());
				glUniform1i(glGetUniformLocation(p, "texsamp"), 0);
				glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);

				glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));
				glUniform1f(glGetUniformLocation(p, "shp"), sin(time));
			}
		}
	}
}
