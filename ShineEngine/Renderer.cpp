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
	delete lp_shader;
}

void CRenderer::Init(GLFWwindow* pWin)
{

	window = pWin;

	m_postprocessor = new CPostProcessor;
	m_postprocessor->Initialize("shaders/quad.ss");

	pSm = new CShadowMapFBO(16000, 16000, "shaders/shadowmap.ss");

	m_pLightSystem = new CLightSystem;
	SMeshParams sd;
	sd.fileName = "objects/sphere.obj";
	sd.name = "dsff";
	sd.m_materialFile = "m.mtl";
	lightsphere = new CMesh(&sd);

	SShaderParams sp;
	sp.name = "kdsad";
	sp.s_file = "shaders/pp_shaders/DeferredLightPass.ss";
	lp_shader = new CShader(&sp);

	SShaderParams ns;
	ns.name = "kdsad";
	ns.s_file = "shaders/null.ss";
	null_shader = new CShader(&ns);

}
void CRenderer::Render()
{
	time += 0.01f;

	//DrawShadowMap();

	glBindFramebuffer(GL_FRAMEBUFFER, m_postprocessor->GetFBO());
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);

	DrawMeshes();
	DrawLights();
	ProcessFramebuffer(m_postprocessor->GetShader()->GetShaderProgramme());

}

void CRenderer::ProcessFramebuffer(GLuint ShaderProg)
{
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_postprocessor->GetFBO());
	//glReadBuffer(GL_COLOR_ATTACHMENT4);
	//glBlitFramebuffer(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1],
	//	0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1], GL_COLOR_BUFFER_BIT, GL_LINEAR);
	//
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//m_postprocessor->GetShader()->Update();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);
	glUseProgram(ShaderProg);

	glUniform2f(static_cast<CShader*>(m_postprocessor->GetShader())->uniformLocations[6], m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);
	m_postprocessor->GetShader()->Update();
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[5]);
	glUniform1i(glGetUniformLocation(ShaderProg, "u_color"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[1]);
	glUniform1i(glGetUniformLocation(ShaderProg, "u_depth"), 4);

	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[4]);
	glUniform1i(glGetUniformLocation(ShaderProg, "u_godraycolor"), 20);

	Vec4 cs = gSys->GetCamera()->GetProjectionMatrix() * (gSys->GetCamera()->GetViewMatrix() * Vec4(500, 300, 500, 1));
	Vec3 ndc = Vec3(cs.x, cs.y, cs.z) / cs.w; 
	Vec2 ss = Vec2(((ndc.x + 1) / 2 * 1280), (ndc.y + 1) / 2 * 720) * Vec2(0.8,1.10);
	glUniform2f(glGetUniformLocation(ShaderProg, "u_lightsspos"), ss.x, ss.y);

	// Bind quad geom buffers
	glBindVertexArray(m_postprocessor->GetQuadVao());
	glDrawElements(GL_TRIANGLES, m_postprocessor->QuadIndices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

#ifdef DEV_MODE
	m_pLightSystem->ProcessLights();
#endif

}

void CRenderer::DrawMeshes()
{
	m_postprocessor->MeshPass();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);

	if (gSys->m_pSkyBox != nullptr)
	{
		gSys->m_pSkyBox->cam = static_cast<CFPCamera*>(gSys->GetCamera());
		//gSys->m_pSkyBox->Draw();
	}

	GLuint p = 0;

	Mat44 vpm = gSys->GetCamera()->GetVPMatrix();

	if (gSys->pMeshSystem != nullptr)
	{
		for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
		{
			if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
			{
				Mat44 wtm = pMesh->GetWorldTM();

				for (Shape* s : pMesh->GetShapeContainer())
				{
					if (s->pMaterial->GetShader())
					{
						p = s->pMaterial->GetShader()->GetShaderProgramme();
						glUseProgram(p);
						s->pMaterial->GetShader()->Update();
					}

					CShader* pProgram = static_cast<CShader*>(s->pMaterial->GetShader()); // The power of interfaces.
					glBindVertexArray(s->meshVao);
					glUniformMatrix4fv(pProgram->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(vpm * wtm));

					// Texture
					glActiveTexture(GL_TEXTURE11);
					glBindTexture(GL_TEXTURE_2D, s->pMaterial->GetTextures()[0]->GetTextureBufferId());
					glUniform1i(pProgram->uniformLocations[1], 11);

					glUniformMatrix4fv(pProgram->uniformLocations[2], 1, GL_FALSE, glm::value_ptr(wtm));
					glUniformMatrix4fv(pProgram->uniformLocations[3], 1, GL_FALSE, glm::value_ptr(DepthBiasMVP));
					glActiveTexture(GL_TEXTURE7);
					glBindTexture(GL_TEXTURE_2D, pSm->GetTextureBufferID());
					glUniform1i(pProgram->uniformLocations[4], 7);

					glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

					glUniform1i(pProgram->uniformLocations[5], gSys->GetCamera()->textures());

				}

				Mat44 depthProjectionMatrix = glm::ortho<float>(-400, 400, -400, 400, -400, 800);
				Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(200, 200, 200), Vec3(0, 0, 0), Vec3(0, 1, 0)) * wtm;
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
	glDepthMask(GL_FALSE);
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
				CShader* pProgram = static_cast<CShader*>(s->pMaterial->GetShader());

				glUseProgram(pSm->GetShader()->GetShaderProgramme());
				Mat44 depthProjectionMatrix = glm::ortho<float>(-400, 400, -400, 400, -400, 800);
				Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(200, 200, 200), Vec3(0, 0, 0), Vec3(0, 1, 0)) * pMesh->GetWorldTM();
				glUniformMatrix4fv(pProgram->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(mvp));
				glBindVertexArray(s->meshVao);
				glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
			}
		}
	}
}

//void CRenderer::DrawGodRayShit()
//{
//	m_postprocessor->GodRayPass();
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(0, 0, 0, 0);
//
//	GLuint p = godray->GetShaderProgramme();
//	glUseProgram(godray->GetShaderProgramme());
//
//	for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
//	{
//		if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
//		{
//			for (Shape* s : pMesh->GetShapeContainer())
//			{
//				CShader* pProgram = static_cast<CShader*>(s->pMaterial->GetShader()); // The power of interfaces.
//				Mat44 mvp = gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM();
//				glUniformMatrix4fv(pProgram->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(mvp));
//				glUniform1i(glGetUniformLocation(p, "asd"), 0);
//				glBindVertexArray(s->meshVao);
//				glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
//			}
//		}
//	}
//
//	for (uint j = 0; j < m_pLightSystem->lightContainer.size(); j++)
//	{
//		Mat44 mvp = gSys->GetCamera()->GetVPMatrix() * glm::translate(Mat44(), m_pLightSystem->lightContainer[j]->GetPos()) * glm::scale(Mat44(), Vec3(100,100,100));
//		glUniformMatrix4fv(static_cast<CShader*>(godray)->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(mvp));
//		glUniform1i(glGetUniformLocation(p, "asd"), 1);
//		glBindVertexArray(lightsphere->GetShapeContainer()[0]->meshVao);
//		glDrawElements(GL_TRIANGLES, lightsphere->GetShapeContainer()[0]->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
//	}
//}

void CRenderer::DrawLights()
{
	// == STENCIL PASS FOR LIGHTS
	m_postprocessor->StencilPass();
	glUseProgram(null_shader->GetShaderProgramme());

	// Render light spheres for the stencil pass
	Mat44 vpm = gSys->GetCamera()->GetVPMatrix();
	for (uint j = 0; j < m_pLightSystem->lightContainer.size(); j++)
	{
		ILight* currentLight = m_pLightSystem->lightContainer[j];
		float scale = m_pLightSystem->GetLightRadius(currentLight);
		glBindVertexArray(lightsphere->GetShapeContainer()[0]->meshVao);

		Mat44 mvp = vpm * glm::translate(Mat44(), m_pLightSystem->lightContainer[j]->GetPos()) * glm::scale(Mat44(), Vec3(scale, scale, scale));
		glUniformMatrix4fv(static_cast<CShader*>(null_shader)->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(mvp));
		glDrawElements(GL_TRIANGLES, lightsphere->GetShapeContainer()[0]->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
	}

	// == LIGHT PASS
	m_postprocessor->LightPass();
	auto p = static_cast<CShader*>(lp_shader)->GetShaderProgramme();
	glUseProgram(p);

	glUniform2f(static_cast<CShader*>(lp_shader)->uniformLocations[6], m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);

	for (uint j = 0; j < m_pLightSystem->lightContainer.size(); j++)
	{
		ILight* currentLight = m_pLightSystem->lightContainer[j];
		float scale = m_pLightSystem->GetLightRadius(currentLight);
		glBindVertexArray(lightsphere->GetShapeContainer()[0]->meshVao);

		Mat44 mvp = vpm * glm::translate(Mat44(), m_pLightSystem->lightContainer[j]->GetPos()) * glm::scale(Mat44(), Vec3(scale, scale, scale));
		glUniformMatrix4fv(static_cast<CShader*>(lp_shader)->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(mvp));
		
		lp_shader->Update();

		// G-Buffers
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[0]);
		glUniform1i(glGetUniformLocation(p, "u_albedo"), 3);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[2]);
		glUniform1i(glGetUniformLocation(p, "u_normaltex"), 5);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_postprocessor->textures[3]);
		glUniform1i(glGetUniformLocation(p, "u_positiontex"), 6);

		// Lights
		glUniform3f(glGetUniformLocation(p, "u_lightPosition"), currentLight->GetPos().x, currentLight->GetPos().y, currentLight->GetPos().z);
		glUniform3f(glGetUniformLocation(p, "u_lightColor"), currentLight->GetColor().x, currentLight->GetColor().y, currentLight->GetColor().z);
		glUniform3f(glGetUniformLocation(p, "u_lightAttenuation"), currentLight->GetAttenuation().x, currentLight->GetAttenuation().y, currentLight->GetAttenuation().z);

		// Camera
		glUniform3f(glGetUniformLocation(p, "u_CamPos"), gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);
		glDrawElements(GL_TRIANGLES, lightsphere->GetShapeContainer()[0]->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
	}

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	glDrawBuffer(GL_BACK);
}



