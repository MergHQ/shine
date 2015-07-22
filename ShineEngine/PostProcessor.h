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

	// 0 = width, 1 = height
	int fbostats[2];
	GLuint textures[4];

	void Initialize();
	GLuint GetFBO() { return fbo; }
	IShader* GetShader() { return pSSRS; }
protected:
private:
	int fbowidth, fboheight;
	GLuint fbo, depthtex, normaltex, quadvao, quadvbo, quadibo, quaduv, colortex, positiontex;
	IShader* pSSRS;
};
#endif