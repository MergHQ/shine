#include "shine.h"
#include "PostProcessor.h"
#include "IShader.h"
#include "Shader.h"
#include "Renderer.h"
#include "Skybox.h"

CPostProcessor::CPostProcessor()
{
}

CPostProcessor::~CPostProcessor()
{
	delete pSSRS;
	glDeleteBuffers(1, &fbo);
	for (uint i = 0; i < 7; i++)
		if(textures[i] != nullptr)
			delete textures[i];
}

void CPostProcessor::Initialize(string shaderfile)
{
	SShaderParams params;
	params.s_file = shaderfile.c_str();
	params.name = "Shadowmap";

	pSSRS = new CShader(&params);

	GLFWwindow* pWin = gSys->pRenderer->GetWin();

	if (pWin)
		glfwGetFramebufferSize(pWin, &fbowidth, &fboheight);

	fbostats[0] = fbowidth;
	fbostats[1] = fboheight;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	colortex = new CFboTexture(GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, fbowidth, fboheight);
	normaltex = new CFboTexture(GL_RGB32F, GL_FLOAT, GL_COLOR_ATTACHMENT1, fbowidth, fboheight);
	positiontex = new CFboTexture(GL_RGB32F, GL_FLOAT, GL_COLOR_ATTACHMENT2, fbowidth, fboheight);
	godray = new CFboTexture(GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT3, fbowidth, fboheight);
	materialParams = new CFboTexture(GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT5, fbowidth, fboheight);
	depthtex = new CFboTexture(GL_DEPTH32F_STENCIL8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, GL_DEPTH_STENCIL_ATTACHMENT, fbowidth, fboheight);
	m_finalTexture = new CFboTexture(GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT4, fbowidth, fboheight);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		gSys->Log("Failed creating framebuffer.");
	}


	textures[0] = colortex;
	textures[1] = depthtex;
	textures[2] = normaltex;
	textures[3] = positiontex;
	textures[4] = godray;
	textures[5] = m_finalTexture;
	textures[6] = materialParams;

	FboQuad();

}

void CPostProcessor::FboQuad()
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
		2, 0, 1, 2, 1, 3
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, QuadIndices.size() * sizeof(uint), &QuadIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(NULL);
}

void CPostProcessor::MeshPass()
{
	GLenum DrawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(5, DrawBuffers);
}

void CPostProcessor::GodRayPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT3);
}

void CPostProcessor::StencilPass()
{
	glDrawBuffer(GL_NONE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);
}

void CPostProcessor::LightPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
}

void CPostProcessor::ProcessFramebuffer()
{
	GLuint ShaderProg = pSSRS->GetShaderProgramme();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, fbostats[0],fbostats[1]);
	glUseProgram(ShaderProg);
	auto castedShader = static_cast<CShader*>(pSSRS);

	glUniform2f(castedShader->uniformLocations[6], fbostats[0], fbostats[1]);
	pSSRS->Update();

	textures[0]->ActivateTexture(GL_TEXTURE3, castedShader->uniformLocations[9], false);
	textures[1]->ActivateTexture(GL_TEXTURE16, castedShader->uniformLocations[11], false);
	textures[2]->ActivateTexture(GL_TEXTURE4, castedShader->uniformLocations[13], false);
	textures[3]->ActivateTexture(GL_TEXTURE20, castedShader->uniformLocations[14], false);
	textures[4]->ActivateTexture(GL_TEXTURE13, castedShader->uniformLocations[12], false);
	textures[5]->ActivateTexture(GL_TEXTURE14, castedShader->uniformLocations[10], false);
	textures[6]->ActivateTexture(GL_TEXTURE15, castedShader->uniformLocations[15], false);
	glUniformMatrix4fv(glGetUniformLocation(castedShader->GetShaderProgramme(), "M_RED"), 1, GL_FALSE, glm::value_ptr(gSys->pEnvironment->GetLightProbeList()[0]->GetIrradianceValues()->m_red));
	glUniformMatrix4fv(glGetUniformLocation(castedShader->GetShaderProgramme(), "M_GREEN"), 1, GL_FALSE, glm::value_ptr(gSys->pEnvironment->GetLightProbeList()[0]->GetIrradianceValues()->m_green));
	glUniformMatrix4fv(glGetUniformLocation(castedShader->GetShaderProgramme(), "M_BLUE"), 1, GL_FALSE, glm::value_ptr(gSys->pEnvironment->GetLightProbeList()[0]->GetIrradianceValues()->m_blue));
	// Camera
	glUniform3f(castedShader->uniformLocations[16], gSys->GetCamera()->GetWorldPos().x, gSys->GetCamera()->GetWorldPos().y, gSys->GetCamera()->GetWorldPos().z);
	if (gSys->pEnvironment->GetLUT() != nullptr)
	{
		glActiveTexture(GL_TEXTURE17);
		glBindTexture(GL_TEXTURE_2D, gSys->pEnvironment->GetLUT()->GetTextureID());
		glUniform1i(glGetUniformLocation(pSSRS->GetShaderProgramme(), "LUT"), 17);
	}
	Vec4 cs = gSys->GetCamera()->GetProjectionMatrix() * (gSys->GetCamera()->GetViewMatrix() * Vec4(500, 500, 500, 1));
	Vec3 ndc = Vec3(cs.x, cs.y, cs.z) / cs.w;
	Vec2 ss = Vec2(((ndc.x + 1) / 2 * 1280), (ndc.y + 1) / 2 * 720) * Vec2(0.79, 1.4);
	glUniform2f(castedShader->uniformLocations[17], ss.x, ss.y);

	// Bind quad.
	glBindVertexArray(GetQuadVao());
	glDrawElements(GL_TRIANGLES, QuadIndices.size() * sizeof(uint), GL_UNSIGNED_INT, 0);
}
