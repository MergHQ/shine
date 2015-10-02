#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H
#pragma once

#include "IShader.h"
#include "shine.h"

#include <GLFW\glfw3.h>
#include <vector>

class CPostProcessor
{
public:
	CPostProcessor();
	~CPostProcessor();

	// 0 = width, 1 = height
	int fbostats[2];
	GLuint textures[7];

	void Initialize(string shaderfile);
	GLuint GetFBO() { return fbo; }
	IShader* GetShader() { return pSSRS; }
	std::vector<GLuint> QuadIndices;
	GLuint GetQuadVao() { return quadvao; }
	void MeshPass();
	void GodRayPass();
	void StencilPass();
protected:
private:
	void FboQuad();
	int fbowidth, fboheight;
	GLuint fbo, depthtex, normaltex, quadvao, quadvbo, quadibo, quaduv, colortex, positiontex, godray, m_depthTexture, m_finalTexture;
	IShader* pSSRS;
};
#endif