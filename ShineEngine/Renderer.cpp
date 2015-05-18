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
		for (unsigned int iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
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
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3, NULL);

			// Textures
			// Create one OpenGL texture
			GLuint textureID;
			glGenTextures(1, &textureID);

			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Give the image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gSys->pMeshSystem->GetMeshContainer()[iter]->width, gSys->pMeshSystem->GetMeshContainer()[iter]->height, 0, GL_BGR, GL_UNSIGNED_BYTE, gSys->pMeshSystem->GetMeshContainer()[iter]->data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


			glDrawElements(GL_TRIANGLES, gSys->pMeshSystem->GetMeshContainer()[iter]->GetIndicies().size(), GL_UNSIGNED_INT, 0);

			glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * gSys->pMeshSystem->GetMeshContainer()[iter]->GetWorldTM()));

		}
		}
	}
