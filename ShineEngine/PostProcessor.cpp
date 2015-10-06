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
	for (uint i = 0; i < sizeof(textures); i++)
		glDeleteTextures(1, &textures[i]);
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


	glGenTextures(1, &depthtex);
	glGenTextures(1, &normaltex);
	glGenTextures(1, &colortex);
	glGenTextures(1, &positiontex);
	glGenTextures(1, &godray);
	glGenTextures(1, &m_finalTexture);
	glGenTextures(1, &m_depthTexture);
	glGenTextures(1, &materialParams);

	glBindTexture(GL_TEXTURE_2D, colortex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbowidth, fboheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colortex, 0);

	glBindTexture(GL_TEXTURE_2D, normaltex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, fbowidth, fboheight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, normaltex, 0);

	glBindTexture(GL_TEXTURE_2D, positiontex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, fbowidth, fboheight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, positiontex, 0);

	glBindTexture(GL_TEXTURE_2D, godray);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbowidth, fboheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, godray, 0);

	glBindTexture(GL_TEXTURE_2D, materialParams);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbowidth, fboheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, materialParams, 0);

	// depth
	glBindTexture(GL_TEXTURE_2D, depthtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, fbowidth, fboheight, 0, GL_DEPTH_STENCIL,
		GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthtex, 0); 

	// final
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbowidth, fboheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, m_finalTexture, 0);

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
	textures[6] = m_depthTexture;
	textures[7] = materialParams;

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
	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT5 };
	glDrawBuffers(4, DrawBuffers);
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
