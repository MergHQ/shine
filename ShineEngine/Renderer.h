#ifndef _RENDERER_H_
#define _RENDERER_H_
#pragma once

#include "IShader.h"
#include "PostProcessor.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
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
protected:
private:
	float time;
	GLFWwindow* window;
	GLuint quadvao, quadvbo, quadibo, quaduv;
	CPostProcessor* postprocessor;
	std::vector<GLuint> QuadIndices;
};
#endif