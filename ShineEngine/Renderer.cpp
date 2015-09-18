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
	delete lightsphere;
	delete godray; 
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
	SMeshParams sd;
	sd.fileName = "objects/sphere.obj";
	sd.name = "dsff";
	sd.m_materialFile = "m.mtl";
	lightsphere = new CMesh(&sd);

	SShaderParams sp;
	sp.name = "kdsad";
	sp.s_file = "shaders/gr.ss";
	godray = new CShader(&sp);

}
void CRenderer::Render()
{
	time += 0.01f;

	DrawShadowMap();

	glBindFramebuffer(GL_FRAMEBUFFER, m_postprocessor->GetFBO());
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);

	glEnable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	DrawGodRayShit();
	DrawMeshes();
	ProcessFramebuffer(m_postprocessor->GetShader()->GetShaderProgramme());
}

void CRenderer::ProcessFramebuffer(GLuint ShaderProg)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);


	glUseProgram(ShaderProg);
	m_postprocessor->GetShader()->Update();

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[0]);
	glUniform1i(glGetUniformLocation(ShaderProg, "diffusetex"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, pSm->GetTextureBufferID());
	glUniform1i(glGetUniformLocation(ShaderProg, "depthtex"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[2]);
	glUniform1i(glGetUniformLocation(ShaderProg, "normaltex"), 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[3]);
	glUniform1i(glGetUniformLocation(ShaderProg, "positiontex"), 6);

	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[4]);
	glUniform1i(glGetUniformLocation(ShaderProg, "godraycolor"), 20);

	Vec4 cs = gSys->GetCamera()->GetProjectionMatrix() * (gSys->GetCamera()->GetViewMatrix() * Vec4(500, 300, 500, 1));
	Vec3 ndc = Vec3(cs.x, cs.y, cs.z) / cs.w;
	Vec2 ss = Vec2(((ndc.x + 1) / 2 * 1280), (ndc.y + 1) / 2 * 720) * Vec2(0.8,1.10);
	glUniform2f(glGetUniformLocation(ShaderProg, "lightsspos"), ss.x, ss.y);

#ifdef DEV_MODE
	m_pLightSystem->ProcessLights();
#endif

	// Lights
	glUniform1i(glGetUniformLocation(ShaderProg, "lightAmount"), m_pLightSystem->lightContainer.size());	

	glUniform3fv(glGetUniformLocation(ShaderProg, "lightPositions"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(m_pLightSystem->positions.data()));
	glUniform3fv(glGetUniformLocation(ShaderProg, "lightColors"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(m_pLightSystem->colors.data()));
	glUniform3fv(glGetUniformLocation(ShaderProg, "lightAttenuations"), m_pLightSystem->lightContainer.size(), reinterpret_cast<GLfloat*>(m_pLightSystem->atts.data()));

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
	m_postprocessor->MeshPass();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);

	if (gSys->m_pSkyBox != nullptr)
	{
		gSys->m_pSkyBox->cam = static_cast<CFPCamera*>(gSys->GetCamera());
		//gSys->m_pSkyBox->Draw();
	}

	GLuint p = 0;

	if (gSys->pMeshSystem != nullptr)
	{
		for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
			{
				for (Shape* s : pMesh->GetShapeContainer())
				{
					// Shader drawing
					if (s->pMaterial->GetShader())
						p = s->pMaterial->GetShader()->GetShaderProgramme();
					glUseProgram(p);
					if (s->pMaterial->GetShader())
						s->pMaterial->GetShader()->Update();

					glBindVertexArray(s->meshVao);

					glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));
					glUniformMatrix4fv(glGetUniformLocation(m_postprocessor->GetShader()->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));

					// Texture
					glActiveTexture(GL_TEXTURE11);
					glBindTexture(GL_TEXTURE_2D, s->pMaterial->GetTextures()[0]->GetTextureBufferId());
					glUniform1i(glGetUniformLocation(p, "texsamp"), 11);

					glUniformMatrix4fv(glGetUniformLocation(p, "Obj2World"), 1, GL_FALSE, glm::value_ptr(pMesh->GetWorldTM()));
					glUniformMatrix4fv(glGetUniformLocation(p, "DepthBias"), 1, GL_FALSE, glm::value_ptr(DepthBiasMVP));
					glActiveTexture(GL_TEXTURE7);
					glBindTexture(GL_TEXTURE_2D, pSm->GetTextureBufferID());
					glUniform1i(glGetUniformLocation(p, "shadowmap"), 7);

					glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

					glUniform1i(glGetUniformLocation(p, "textures"), gSys->GetCamera()->textures());
				}

				Mat44 depthProjectionMatrix = glm::ortho<float>(-400, 400, -400, 400, -400, 800);
				Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(50, 50, 50), Vec3(0, 0, 0), Vec3(0, 1, 0)) * pMesh->GetWorldTM();
				glm::mat4 biasMatrix(
					0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0
					);
				DepthBiasMVP = biasMatrix*mvp;
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
			for (Shape* s : pMesh->GetShapeContainer())
			{
				glUseProgram(pSm->GetShader()->GetShaderProgramme());
				Mat44 depthProjectionMatrix = glm::ortho<float>(-400, 400, -400, 400, -400, 800);
				Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(700, 200, 700), Vec3(0, 0, 0), Vec3(0, 1, 0)) * pMesh->GetWorldTM();
				glUniformMatrix4fv(glGetUniformLocation(pSm->GetShader()->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
				glBindVertexArray(s->meshVao);
				glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
			}
		}
	}
}

void CRenderer::DrawGodRayShit()
{
	m_postprocessor->GodRayPass();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	GLuint p = godray->GetShaderProgramme();
	glUseProgram(godray->GetShaderProgramme());

	for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
	{
		if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
		{
			for (Shape* s : pMesh->GetShapeContainer())
			{
				Mat44 mvp = gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM();
				glUniformMatrix4fv(glGetUniformLocation(godray->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
				glUniform1i(glGetUniformLocation(p, "asd"), 0);
				glBindVertexArray(s->meshVao);
				glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
			}
		}
	}

	for (uint j = 0; j < m_pLightSystem->lightContainer.size(); j++)
	{
		Mat44 mvp = gSys->GetCamera()->GetVPMatrix() * glm::translate(Mat44(), m_pLightSystem->lightContainer[j]->GetPos()) * glm::scale(Mat44(), Vec3(100,100,100));
		glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform1i(glGetUniformLocation(p, "asd"), 1);
		glBindVertexArray(lightsphere->GetShapeContainer()[0]->meshVao);
		glDrawElements(GL_TRIANGLES, lightsphere->GetShapeContainer()[0]->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
	}
}



