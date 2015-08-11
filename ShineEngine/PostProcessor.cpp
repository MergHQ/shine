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

	glGenRenderbuffers(1, &depthtex);
	glBindRenderbuffer(GL_RENDERBUFFER, depthtex);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbowidth, fboheight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthtex);

	glBindTexture(GL_TEXTURE_2D, depthtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, fbowidth, fboheight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtex, 0);

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

	GLenum DrawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
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