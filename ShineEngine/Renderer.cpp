#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"
#include "Camera.h"
#include "FPCamera.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

CRenderer::CRenderer():
time(0)
{
}

CRenderer::~CRenderer()
{
	ReleaseSystems();
}

void CRenderer::ReleaseSystems()
{
	delete m_postprocessor;
	delete m_pLightSystem;
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

	m_postprocessor = new CPostProcessor;	
	m_postprocessor->Initialize("shaders/pp_shaders/DeferredLightPass.ss");

	m_pLightSystem = new CLightSystem;

	FboQuad();
}
void CRenderer::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_postprocessor->GetFBO());
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);

	glEnable(GL_LIGHTING);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	DrawMeshes();
	ProcessFramebuffer(m_postprocessor->GetShader()->GetShaderProgramme());
}

void CRenderer::ProcessFramebuffer(GLuint ShaderProg)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ShaderProg);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[0]);
	glUniform1i(glGetUniformLocation(ShaderProg, "diffusetex"), 3);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[1]);
	glUniform1i(glGetUniformLocation(ShaderProg, "depthtex"), 4);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[2]);
	glUniform1i(glGetUniformLocation(ShaderProg, "normaltex"), 5);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[3]);
	glUniform1i(glGetUniformLocation(ShaderProg, "positiontex"), 6);
	glActiveTexture(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE7);
	//glBindTexture(GL_TEXTURE_2D, m_shadowMapProcessor->textures[1]);
	//glUniform1i(glGetUniformLocation(ShaderProg, "shadowmpapos"), 7);
	//glActiveTexture(GL_TEXTURE0);

	// Lights
	glUniform1i(glGetUniformLocation(ShaderProg, "lightAmount"), m_pLightSystem->lightContainer.size());	

	glUniform3fv(glGetUniformLocation(ShaderProg, "lightPositions"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(m_pLightSystem->positions.data()));
	glUniform3fv(glGetUniformLocation(ShaderProg, "lightColors"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(m_pLightSystem->colors.data()));
	glUniform3fv(glGetUniformLocation(ShaderProg, "lightAtteniuations"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(m_pLightSystem->atts.data()));

	glUniform1fv(glGetUniformLocation(ShaderProg, "lightUsesShadows"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(m_pLightSystem->shadows.data()));
	glUniform1iv(glGetUniformLocation(ShaderProg, "lightTypes"), m_pLightSystem->lightContainer.size(), m_pLightSystem->types.data());

	// Camera
	glUniform3f(glGetUniformLocation(ShaderProg, "CamPos"), gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);

	// Bind quad geom buffers
	glBindVertexArray(quadvao);
	glDrawElements(GL_TRIANGLES, QuadIndices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
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
				glUniformMatrix4fv(glGetUniformLocation(m_postprocessor->GetShader()->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));

				// Textures
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, pMesh->GetMaterial()->GetTextures()[0]->GetTextureId());
				glUniform1i(glGetUniformLocation(p, "texsamp"), 11);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE12);
				glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[0]);
				glUniform1i(glGetUniformLocation(p, "diffusetex"), 12);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE13);
				glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[1]);
				glUniform1i(glGetUniformLocation(p, "depthtex"), 13);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE14);
				glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[2]);
				glUniform1i(glGetUniformLocation(p, "normaltex"), 14);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE15);
				glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[3]);
				glUniform1i(glGetUniformLocation(p, "positiontex"), 15);
				glActiveTexture(GL_TEXTURE0);

				glUniformMatrix4fv(glGetUniformLocation(p, "Obj2World"), 1, GL_FALSE, glm::value_ptr(pMesh->GetWorldTM()));

				glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);

				glUniform1i(glGetUniformLocation(p, "textures"), gSys->GetCamera()->textures());

			}
		}
	}
}



