#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#pragma once

#include "IShader.h"
#include <GLFW\glfw3.h>

class CPostProcessor
{
public:
	CPostProcessor();
	~CPostProcessor();

	void Initialize();
	GLuint GetFBO() { return fbo; }
	GLuint textures[4];
	// 0 = width, 1 = height
	int fbostats[2];
	IShader* GetShader() { return pSSRS; }

private:
	GLuint fbo, depthtex, normaltex, quadvao, quadvbo, quadibo, quaduv, colortex, positiontex;
	int fbowidth, fboheight;
	IShader* pSSRS;
};

#endif