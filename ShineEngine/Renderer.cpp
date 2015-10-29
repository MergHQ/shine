#include "Renderer.h"
#include "GlobalSystem.h"
#include "MeshSystem.h"
#include "Camera.h"
#include "FPCamera.h"
#include "Skybox.h"
#include "EditorController.h"

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
	delete pProbeGen;
}

void CRenderer::Init(GLFWwindow* pWin)
{
	window = pWin;

	m_postprocessor = new CPostProcessor;
	m_postprocessor->Initialize("shaders/quad.ss");

	pSm = new CShadowMapFBO(6144, 6144, "shaders/shadowmap.ss");

	std::vector<string> a{ "irradiance/posx.jpg","irradiance/negx.jpg", "irradiance/posy.jpg","irradiance/negy.jpg","irradiance/posz.jpg","irradiance/negz.jpg" };
	pProbeGen = new CIlluminationProbe(a, o1,o2,o3);

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

	SShaderParams nska;
	nska.name = "kdsad";
	nska.s_file = "shaders/gr.ss";
	godray = new CShader(&nska);

	for (uint i = 0; i < 64; i++) {
		float scale = (float)i / (float)(64);
		Vec3 v;
		v.x = 2.0f * (float)rand() / RAND_MAX - 1.0f;
		v.y = 2.0f * (float)rand() / RAND_MAX - 1.0f;
		v *= (0.1f + 0.9f * scale * scale);

		kernelPoints[i] = v;
	}

}

void CRenderer::Render()
{
	time += 0.001f;


	//DrawShadowMap();

	glBindFramebuffer(GL_FRAMEBUFFER, m_postprocessor->GetFBO());
	glViewport(0, 0, m_postprocessor->fbostats[0], m_postprocessor->fbostats[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);
	glEnable(GL_FRAMEBUFFER_SRGB);
	if (gSys->m_pSkyBox != nullptr)
	{
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		gSys->m_pSkyBox->cam = static_cast<CFPCamera*>(gSys->GetCamera());
		gSys->m_pSkyBox->Draw();
	}
	DrawMeshes();
	DrawLights();
#ifdef DEV_MODE
	gSys->pEditorController->DrawEditorObjects(); // Draw editor helpers
#endif
	m_postprocessor->ProcessFramebuffer();

}

void CRenderer::DrawMeshes()
{
	m_postprocessor->MeshPass();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);

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
					gSys->m_pSkyBox->tex->ActivateTexture(GL_TEXTURE20, glGetUniformLocation(pProgram->GetShaderProgramme(), "cubemapsamp"), true);
					glUniform3f(pProgram->uniformLocations[16], gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);

					// Diffuse
					if (s->pMaterial->GetTextures().size() > 0)
					{
						s->pMaterial->GetTextures()[0]->ActivateTexture(GL_TEXTURE11, pProgram->uniformLocations[1], false);
					}

					// Normal
					if (s->pMaterial->GetTextures().size() > 1)
					{
						s->pMaterial->GetTextures()[1]->ActivateTexture(GL_TEXTURE12, pProgram->uniformLocations[8], false);
					}


					glUniformMatrix4fv(pProgram->uniformLocations[2], 1, GL_FALSE, glm::value_ptr(wtm));
					glUniformMatrix4fv(pProgram->uniformLocations[3], 1, GL_FALSE, glm::value_ptr(DepthBiasMVP));
					glActiveTexture(GL_TEXTURE7);
					glBindTexture(GL_TEXTURE_2D, pSm->GetTextureBufferID());
					glUniform1i(pProgram->uniformLocations[4], 7);

					glUniform1f(glGetUniformLocation(pProgram->GetShaderProgramme(), "time"), time);

					glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

					glUniform1i(pProgram->uniformLocations[5], gSys->GetCamera()->textures());
					// Material params

					// Roughness
					glUniform1f(pProgram->uniformLocations[7], s->pMaterial->GetMaterialParams()[1]);

					m_pLastMesh = pMesh;

				}


				Mat44 depthProjectionMatrix = glm::ortho<float>(-100, 100, -100, 100, -100, 200);
				Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(50, 50, 50), Vec3(0, 0, 0), Vec3(0, 1, 0)) * wtm;
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

	DrawGodRayShit();
	glDepthMask(GL_FALSE);
}

void CRenderer::DrawShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, pSm->GetFBO());
	glViewport(0, 0, 6144, 6144);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (uint iter = 0; iter < gSys->pMeshSystem->GetMeshContainer().size(); iter++)
	{
		if (IMesh* pMesh = gSys->pMeshSystem->GetMeshContainer()[iter])
		{
			for (Shape* s : pMesh->GetShapeContainer())
			{
				CShader* pProgram = static_cast<CShader*>(pSm->GetShader());

				glUseProgram(pSm->GetShader()->GetShaderProgramme());
				Mat44 depthProjectionMatrix = glm::ortho<float>(-100, 100, -100, 100, -100, 200);
				Mat44 mvp = depthProjectionMatrix * glm::lookAt(Vec3(50, 50, 50), Vec3(0, 0, 0), Vec3(0, 1, 0)) * pMesh->GetWorldTM();
				glUniformMatrix4fv(pProgram->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(mvp));
				glBindVertexArray(s->meshVao);
				glDrawElements(GL_TRIANGLES, s->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
			}
		}
	}
	glDepthMask(GL_FALSE);
}

void CRenderer::DrawGodRayShit()
{
	m_postprocessor->GodRayPass();
	glClearColor(0, 0, 0, 0);

	GLuint p = godray->GetShaderProgramme();
	glUseProgram(godray->GetShaderProgramme());

	godray->Update();

	Mat44 asdd = gSys->GetCamera()->GetVPMatrix() * glm::translate(Mat44(), Vec3(500, 500, 500)) * glm::scale(Mat44(), Vec3(60, 60, 60));
	glUniformMatrix4fv(static_cast<CShader*>(godray)->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(asdd));
	glUniform1i(glGetUniformLocation(p, "asd"), 1);
	glBindVertexArray(lightsphere->GetShapeContainer()[0]->meshVao);
	glDrawElements(GL_TRIANGLES, lightsphere->GetShapeContainer()[0]->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
}

void CRenderer::DrawLights()
{
#ifdef DEV_MODE
	m_pLightSystem->ProcessLights();
#endif

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
		m_postprocessor->textures[0]->ActivateTexture(GL_TEXTURE3, static_cast<CShader*>(lp_shader)->uniformLocations[10], false);
		m_postprocessor->textures[2]->ActivateTexture(GL_TEXTURE5, static_cast<CShader*>(lp_shader)->uniformLocations[13], false);
		m_postprocessor->textures[3]->ActivateTexture(GL_TEXTURE6, static_cast<CShader*>(lp_shader)->uniformLocations[14], false);
		m_postprocessor->textures[6]->ActivateTexture(GL_TEXTURE7, static_cast<CShader*>(lp_shader)->uniformLocations[15], false);

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

}



