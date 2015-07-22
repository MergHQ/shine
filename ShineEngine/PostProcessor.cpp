#include "shine.h"
#include "PostProcessor.h"
#include "IShader.h"
#include "Shader.h"
#include "Renderer.h"

CPostProcessor::CPostProcessor()
{
}

CPostProcessor::~CPostProcessor()
{
	delete pSSRS;
	glDeleteBuffers(1, &fbo);
	glDeleteBuffers(1, &depthtex);
	glDeleteBuffers(1, &normaltex);
	glDeleteBuffers(1, &colortex);
	glDeleteBuffers(1, &positiontex);
}

void CPostProcessor::Initialize()
{
	SShaderParams params;
	params.s_file = "shaders/pp_shaders/DeferredRenderPass.ss";
	params.name = "DefferedRenderShader";

	pSSRS = new CShader(&params);

	GLFWwindow* pWin = gSys->pRenderer->GetWin();

	if (pWin)
		glfwGetFramebufferSize(pWin, &fbowidth, &fboheight);

	fbostats[0] = fbowidth;
	fbostats[1] = fboheight;

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
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, colortex, 0);

	glBindTexture(GL_TEXTURE_2D, normaltex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbowidth, fboheight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1_EXT, normaltex, 0);

	glBindTexture(GL_TEXTURE_2D, positiontex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbowidth, fboheight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2_EXT, positiontex, 0);


	GLenum DrawBuffers[3] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glDrawBuffers(3, DrawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		gSys->Log("Failed creating framebuffer.");
	}

	textures[0] = colortex;
	textures[1] = depthtex;
	textures[2] = normaltex;
	textures[3] = positiontex;
}