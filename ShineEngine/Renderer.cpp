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
	delete pSSRS;
	glDeleteBuffers(1, &fbo);
	glDeleteBuffers(1, &depthtex);
	glDeleteBuffers(1, &normaltex);
	glDeleteBuffers(1, &quadvao);
	glDeleteBuffers(1, &quadibo);
	glDeleteBuffers(1, &quadvbo);
	glDeleteBuffers(1, &quaduv);
	glDeleteBuffers(1, &colortex);

}

void CRenderer::Init(GLFWwindow* pWin)
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//TEMP: SSR Shader
	SShaderParams params;
	params.s_file = "shaders/pp_shaders/ssr.ss";
	params.name = "ScreenSpaceReflection";

	window = pWin;

	pSSRS = new CShader(&params);

	if (window)
		glfwGetFramebufferSize(window, &fbowidth, &fboheight);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &depthtex);
	glGenTextures(1, &normaltex);
	glGenTextures(1, &colortex);

	glGenRenderbuffers(1, &depthtex);
	glBindRenderbuffer(GL_RENDERBUFFER, depthtex);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbowidth, fboheight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthtex);

	glBindTexture(GL_TEXTURE_2D, depthtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, fbowidth, fboheight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtex, 0);

	glBindTexture(GL_TEXTURE_2D, colortex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbowidth, fboheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colortex, 0);

	glBindTexture(GL_TEXTURE_2D, normaltex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbowidth, fboheight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, normaltex, 0);

	 
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		gSys->Log("Failed creating framebuffer.");
	}

	FboQuad();
}

void CRenderer::Render()
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, fbowidth, fboheight);

	glEnable(GL_LIGHTING);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	time += 0.008f;

	GLuint p = 0;

	DrawMeshes();
	ProcessFramebuffer(pSSRS->GetShaderProgramme());

}

void CRenderer::ProcessFramebuffer(GLuint ShaderProg)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, fbowidth, fboheight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pSSRS->GetShaderProgramme());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glUniform1i(glGetUniformLocation(pSSRS->GetShaderProgramme(), "diffusetex"), 3);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, depthtex);
	glUniform1i(glGetUniformLocation(pSSRS->GetShaderProgramme(), "depthtex"), 4);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, normaltex);
	glUniform1i(glGetUniformLocation(pSSRS->GetShaderProgramme(), "normaltex"), 5);
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
				glUniformMatrix4fv(glGetUniformLocation(pSSRS->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * pMesh->GetWorldTM()));

				// Textures
				// Create one OpenGL texture
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, pMesh->GetMaterial()->GetTextures()[0]->GetTextureId());
				glUniform1i(glGetUniformLocation(p, "texsamp"), 11);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, colortex);
				glUniform1i(glGetUniformLocation(p, "diffusetex"), 3);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, depthtex);
				glUniform1i(glGetUniformLocation(p, "depthtex"), 4);
				glActiveTexture(GL_TEXTURE0);

				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D, normaltex);
				glUniform1i(glGetUniformLocation(p, "normaltex"), 5);
				glActiveTexture(GL_TEXTURE0);

				glUniformMatrix4fv(glGetUniformLocation(p, "Obj2World"), 1, GL_FALSE, glm::value_ptr(pMesh->GetWorldTM()));
				glm::mat3 inv_transp = glm::transpose(glm::inverse(glm::mat3(gSys->GetCamera()->GetViewMatrix() * pMesh->GetWorldTM())));
				glUniform3fv(glGetUniformLocation(p, "normal_matrix"), 1, glm::value_ptr(inv_transp));
				glUniform3f(glGetUniformLocation(p, "CamPosW"), gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);
				glUniform1f(glGetUniformLocation(p, "shp"), sin(time));
				glUniformMatrix4fv(glGetUniformLocation(p, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetProjectionMatrix()));

				glDrawElements(GL_TRIANGLES, pMesh->GetIndicies().size() * sizeof(uint), GL_UNSIGNED_INT, 0);
			}
		}
	}
}



