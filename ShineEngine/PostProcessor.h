#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H
#pragma once

#include "IShader.h"
#include "shine.h"
#include "ITexture.h"

#include <GLFW\glfw3.h>
#include <vector>

class CPostProcessor
{
public:
	CPostProcessor();
	~CPostProcessor();

	// 0 = width, 1 = height
	int fbostats[2];
	ITexture* textures[7];

	void Initialize(string shaderfile);
	GLuint GetFBO() { return fbo; }
	IShader* GetShader() { return pSSRS; }
	std::vector<GLuint> QuadIndices;
	GLuint GetQuadVao() { return quadvao; }
	void MeshPass();
	void GodRayPass();
	void StencilPass();
	void LightPass();
	void ProcessFramebuffer();
protected:
private:
	void FboQuad();
	int fbowidth, fboheight;
	ITexture* depthtex;
	ITexture* normaltex; 
	ITexture* colortex;
	ITexture* positiontex;
	ITexture* godray;
	ITexture* m_depthTexture;
	ITexture* m_finalTexture;
	ITexture* materialParams;
	GLuint quadvao, quadvbo, quadibo, quaduv, fbo;
	IShader* pSSRS;
};
#endif