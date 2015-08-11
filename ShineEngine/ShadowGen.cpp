#include "ShadowGen.h"
#include "Shader.h"

CShadowMapFBO::CShadowMapFBO(int width, int height, string shader)
{
	SShaderParams p;
	p.name = "ShadowMapShader";
	p.s_file = shader.c_str();

	pShader = new CShader(&p);

	textureWidth = width;
	textureHeight = height;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &temptex);
	glBindTexture(GL_TEXTURE_2D, temptex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, temptex, 0);

	textureid = temptex;

	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		gSys->Log("Error creating shadowmap FBO :(");
}

CShadowMapFBO::~CShadowMapFBO()
{
	glDeleteTextures(1, &textureid);
	glDeleteTextures(1, &temptex);
	glDeleteFramebuffers(1, &fbo);
	delete pShader;
}