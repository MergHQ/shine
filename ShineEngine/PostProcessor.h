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
	GLuint textures[4];

	void Initialize(string shaderfile, bool shadowmap);
	GLuint GetFBO() { return fbo; }
	IShader* GetShader() { return pSSRS; }
	std::vector<GLuint> QuadIndices;
	GLuint GetQuadVao() { return quadvao; }
protected:
private:
	void FboQuad();
	int fbowidth, fboheight;
	GLuint fbo, depthtex, normaltex, quadvao, quadvbo, quadibo, quaduv, colortex, positiontex;
	IShader* pSSRS;
};
#endif