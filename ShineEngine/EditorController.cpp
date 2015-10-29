#include "EditorController.h"
#include <GLFW\glfw3.h>
#include <gl\glew.h>
#include "ILight.h"
#include "Shader.h"
#include "LightSystem.h"
#include "Mesh.h"
#include "Renderer.h"

#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


CEditorController::CEditorController()
{
}

CEditorController::~CEditorController()
{
}

void CEditorController::Init()
{
	SMeshParams sd;
	sd.fileName = "objects/sphere.obj";
	sd.name = "dsff";
	sd.m_materialFile = "m.mtl";
	m_pSphere = new CMesh(&sd);


	SShaderParams dsadf;
	dsadf.name = "dsadfds";
	dsadf.s_file = "shaders/simple.ss";
	m_pHelperShader = new CShader(&dsadf);
}

void CEditorController::DrawEditorObjects()
{

	// Draw light helpers
	CLightSystem* pLightSys = gSys->pRenderer->GetLightSystem();
	auto p2 = static_cast<CShader*>(m_pHelperShader)->GetShaderProgramme();
	glUseProgram(p2);
	Mat44 vpm = gSys->GetCamera()->GetVPMatrix();
	for (uint j = 0; j < pLightSys->lightContainer.size(); j++)
	{
		ILight* currentLight = pLightSys->lightContainer[j];
		float scale = pLightSys->GetLightRadius(currentLight);
		glBindVertexArray(m_pSphere->GetShapeContainer()[0]->meshVao);

		Mat44 mvp = vpm * glm::translate(Mat44(), pLightSys->lightContainer[j]->GetPos()) * glm::scale(Mat44(), Vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(static_cast<CShader*>(m_pHelperShader)->uniformLocations[0], 1, GL_FALSE, glm::value_ptr(mvp));
		// Lights
		glUniform3f(glGetUniformLocation(p2, "u_lightColor"), currentLight->GetColor().x, currentLight->GetColor().y, currentLight->GetColor().z);

		glDrawElements(GL_TRIANGLES, m_pSphere->GetShapeContainer()[0]->indices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
	}
}
