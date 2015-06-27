#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "IShader.h"
#include <vector>

#define BUFFER_OFFSET(i) ((void*)(i))

class CRenderer
{

public:
	CRenderer();
	~CRenderer();
	void Init(GLFWwindow* pWin);
	void Render();
	void ProcessFramebuffer(GLuint ShaderProg); 
	void FboQuad();
	void DrawMeshes();
	GLFWwindow* GetWin() { return window; }
private:
	GLFWwindow* window;
	IShader* pSSRS;
	float time;
	int fbowidth, fboheight;
	GLuint fbo, depthtex, normaltex, quadvao, quadvbo, quadibo, quaduv, colortex;
	std::vector<GLuint> QuadIndices;
};

#endif