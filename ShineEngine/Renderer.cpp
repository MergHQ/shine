#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"
#include "Camera.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CRenderer::CRenderer():
time(0)
{
}

CRenderer::~CRenderer()
{
	delete postprocessor;
	glDeleteBuffers(1, &quadvao);
	glDeleteBuffers(1, &quadibo);
	glDeleteBuffers(1, &quadvbo);
	glDeleteBuffers(1, &quaduv);
}

void CRenderer::Init(GLFWwindow* pWin)
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	window = pWin;

	postprocessor = new CPostProcessor;
	postprocessor->Initialize();
	 

	FboQuad();
}

void CRenderer::Render()
{

	glBindFramebuffer(GL_FRAMEBUFFER, postprocessor->GetFBO());
	glViewport(0, 0, postprocessor->fbostats[0], postprocessor->fbostats[1]);

	glEnable(GL_LIGHTING);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	time += 0.008f;

	GLuint p = 0;

	DrawMeshes();
	ProcessFramebuffer(postprocessor->GetShader()->GetShaderProgramme());

}

void CRenderer::ProcessFramebuffer(GLuint ShaderProg)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, postprocessor->fbostats[0], postprocessor->fbostats[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(postprocessor->GetShader()->GetShaderProgramme());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, postprocessor->textures[0]);
	glUniform1i(glGetUniformLocation(postprocessor->GetShader()->GetShaderProgramme(), "diffusetex"), 3);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, postprocessor->textures[1]);
	glUniform1i(glGetUniformLocation(postprocessor->GetShader()->GetShaderProgramme(), "depthtex"), 4);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, postprocessor->textures[2]);
	glUniform1i(glGetUniformLocation(postprocessor->GetShader()->GetShaderProgramme(), "normaltex"), 5);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, postprocessor->textures[3]);
	glUniform1i(glGetUniformLocation(postprocessor->GetShader()->GetShaderProgramme(), "positiontex"), 6);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(quadvao);
	glDrawElements(GL_TRIANGLES, QuadIndices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

	// Draw again without drawing to the framebuffer
	//DrawMeshes();
}

void CRenderer::FboQuad()
{
	// INDEX
	//   2________3
	//	 |\		 |
	//	 |	 \	 |
	//	 |______\|
	//   0		 1

	const GLfloat QuadVerts[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	QuadIndices =
	{
		2,0,1,2,1,3
	};

	const GLfloat UV[] =
	{
		0, 0, 1, 0, 0, 1, 1, 1
	};

	glGenVertexArrays(1, &quadvao);
	glBindVertexArray(quadvao);

	glEnableVertexAttribArray(0);
	glGenBuffers(1, &quadvao);
	glBindBuffer(GL_ARRAY_BUFFER, quadvao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(QuadVerts), &QuadVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, (GLubyte *)NULL);


	glGenBuffers(1, &quadibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, QuadIndices.size() * sizeof(uint), &QuadIndices[0] , GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glGenBuffers(1, &quaduv);
	glBindBuffer(GL_ARRAY_BUFFER, quaduv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UV) * sizeof(GLfloat), &UV, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(NULL);
}

void CRenderer::DrawMeshes()
{
	GLuint p = 0;

	if (gSys->pMeshSystem != nullptr)
	{
		for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
			{
				// Shader drawing
				if (pMesh->GetMaterial()->GetShader())
					p = pMesh->GetMaterial()->GetShader()->GetShaderProgramme();
				glUseProgram(p);
				if (pMesh->GetMaterial()->GetShader())
					pMesh->GetMaterial()->GetShader()->Update();

				glBindVertexArray(pMesh->GetVao());

				glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));
				glUniformMatrix4fv(glGetUniformLocation(postprocessor->GetShader()->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));

				// Textures
				// Create one OpenGL texture
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, pMesh->GetMaterial()->GetTextures()[0]->GetTextureId());
				glUniform1i(glGetUniformLocation(p, "texsamp"), 11);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, postprocessor->textures[0]);
				glUniform1i(glGetUniformLocation(p, "diffusetex"), 3);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, postprocessor->textures[1]);
				glUniform1i(glGetUniformLocation(p, "depthtex"), 4);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D, postprocessor->textures[2]);
				glUniform1i(glGetUniformLocation(p, "normaltex"), 5);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE6);
				glBindTexture(GL_TEXTURE_2D, postprocessor->textures[3]);
				glUniform1i(glGetUniformLocation(p, "positiontex"), 6);
				glActiveTexture(GL_TEXTURE0);

				glUniformMatrix4fv(glGetUniformLocation(p, "Obj2World"), 1, GL_FALSE, glm::value_ptr(pMesh->GetWorldTM()));
				glm::mat3 inv_transp = glm::mat3(glm::inverseTranspose(gSys->GetCamera()->GetViewMatrix() * pMesh->GetWorldTM()));
				glUniformMatrix3fv(glGetUniformLocation(p, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(inv_transp));
				glUniform3f(glGetUniformLocation(p, "CamPosW"), gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);
				glUniform1f(glGetUniformLocation(p, "shp"), sin(time));
				glUniformMatrix4fv(glGetUniformLocation(p, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetProjectionMatrix()));

				glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);
			}
		}
	}
}



