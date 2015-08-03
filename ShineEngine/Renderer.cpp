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
	time += 0.008f;

	GLuint p = 0;

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

	// Lights
	glUniform1i(glGetUniformLocation(ShaderProg, "lightAmount"), m_pLightSystem->lightContainer.size());

	std::vector<Vec3> positions;
	std::vector<Vec3> colors;
	std::vector<Vec3> atts;
	std::vector<int> lighttypes;
	std::vector<int> useshadows;

	for (uint j = 0; j < m_pLightSystem->lightContainer.size(); j++)
	{
		if (ILight* pLight = m_pLightSystem->lightContainer[j])
		{

			positions.push_back(pLight->GetPos());
			colors.push_back(pLight->GetColor());
			atts.push_back(pLight->GetAttenuation());

			if (pLight->IsShadowsEnabled())
				useshadows.push_back(1);
			else
				useshadows.push_back(0);

			if (pLight->GetType() == POINTLIGHT)
				lighttypes.push_back(1);
			else
				lighttypes.push_back(0);
		}
	}

	glUniform3fv(glGetUniformLocation(ShaderProg, "lightPositions"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(positions.data()));
	glUniform3fv(glGetUniformLocation(ShaderProg, "lightColors"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(colors.data()));
	glUniform3fv(glGetUniformLocation(ShaderProg, "lightAtteniuations"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(atts.data()));

	glUniform1fv(glGetUniformLocation(ShaderProg, "lightUsesShadows"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(useshadows.data()));
	glUniform1iv(glGetUniformLocation(ShaderProg, "lightTypes"), m_pLightSystem->lightContainer.size(), lighttypes.data());

	glUniform3f(glGetUniformLocation(ShaderProg, "CamPos"), gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);

	glBindVertexArray(quadvao);
	glDrawElements(GL_TRIANGLES, QuadIndices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

	positions.clear();
	colors.clear();
	atts.clear();
	useshadows.clear();
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
				// Create one OpenGL texture
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
				glm::mat3 inv_transp = glm::mat3(glm::inverseTranspose(gSys->GetCamera()->GetViewMatrix() * pMesh->GetWorldTM()));
				glUniformMatrix3fv(glGetUniformLocation(p, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(inv_transp));
				glUniform3f(glGetUniformLocation(p, "CamPosW"), gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);
				glUniform1f(glGetUniformLocation(p, "shp"), sin(time));
				glUniformMatrix4fv(glGetUniformLocation(p, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetProjectionMatrix()));
				glUniformMatrix4fv(glGetUniformLocation(p, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetViewMatrix()));

				glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);

				glUniform1i(glGetUniformLocation(p, "textures"), gSys->GetCamera()->textures());

			}
		}
	}
}



