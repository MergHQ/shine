#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"
#include "Camera.h"
#include "FPCamera.h"
#include "Skybox.h"

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
	delete pSm;
}

void CRenderer::Init(GLFWwindow* pWin)
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	window = pWin;

	m_postprocessor = new CPostProcessor;
	m_postprocessor->Initialize("shaders/pp_shaders/DeferredLightPass.ss");

	pSm = new CShadowMapFBO(16000, 16000, "shaders/shadowmap.ss");

	m_pLightSystem = new CLightSystem;

}
void CRenderer::Render()
{
	time += 0.01f;

	DrawShadowMap();

	glBindFramebuffer(GL_FRAMEBUFFER, m_postprocessor->GetFBO());
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);

	glEnable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	DrawMeshes();
	ProcessFramebuffer(m_postprocessor->GetShader()->GetShaderProgramme());
}

void CRenderer::ProcessFramebuffer(GLuint ShaderProg)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glUseProgram(ShaderProg);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[0]);
	glUniform1i(glGetUniformLocation(ShaderProg, "diffusetex"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[1]);
	glUniform1i(glGetUniformLocation(ShaderProg, "depthtex"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[2]);
	glUniform1i(glGetUniformLocation(ShaderProg, "normaltex"), 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[3]);
	glUniform1i(glGetUniformLocation(ShaderProg, "positiontex"), 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, pSm->GetTextureBufferID());
	glUniform1i(glGetUniformLocation(ShaderProg, "shadowmpapos"), 7);

#ifdef DEV_MODE
	m_pLightSystem->ProcessLights();
#endif

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
	glBindVertexArray(m_postprocessor->GetQuadVao());
	glDrawElements(GL_TRIANGLES, m_postprocessor->QuadIndices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

}

void CRenderer::DrawMeshes()
{
	if (gSys->m_pSkyBox != nullptr)
	{
		gSys->m_pSkyBox->cam = static_cast<CFPCamera*>(gSys->GetCamera());
		gSys->m_pSkyBox->Draw();
	}

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

				// Texture
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, pMesh->GetMaterial()->GetTextures()[0]->GetTextureBufferId());
				glUniform1i(glGetUniformLocation(p, "texsamp"), 11);

				glUniformMatrix4fv(glGetUniformLocation(p, "Obj2World"), 1, GL_FALSE, glm::value_ptr(pMesh->GetWorldTM()));

				Mat44 depthProjectionMatrix = glm::ortho<float>(-400, 400, -400, 400, -400, 800);
				Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(50, 50, 50), Vec3(0, 0, 0), Vec3(0, 1, 0)) * pMesh->GetWorldTM();
				glm::mat4 biasMatrix(
					0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0
					);
				DepthBiasMVP = biasMatrix*mvp;

				glUniformMatrix4fv(glGetUniformLocation(p, "DepthBias"), 1, GL_FALSE, glm::value_ptr(DepthBiasMVP));

				glActiveTexture(GL_TEXTURE7);
				glBindTexture(GL_TEXTURE_2D, pSm->GetTextureBufferID());
				glUniform1i(glGetUniformLocation(p, "shadowmap"), 7);

				glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);

				glUniform1i(glGetUniformLocation(p, "textures"), gSys->GetCamera()->textures());

			}
		}
	}
}

void CRenderer::DrawShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, pSm->GetFBO());
	glViewport(0, 0, 16000, 16000);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
	{
		if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
		{
			glUseProgram(pSm->GetShader()->GetShaderProgramme());
			Mat44 depthProjectionMatrix = glm::ortho<float>(-400, 400, -400, 400, -400, 800);
			Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(50, 50, 50), Vec3(0, 0, 0), Vec3(0, 1, 0)) * pMesh->GetWorldTM();
			glUniformMatrix4fv(glGetUniformLocation(pSm->GetShader()->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
			glBindVertexArray(pMesh->GetVao());
			glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);
		}
	}
}



