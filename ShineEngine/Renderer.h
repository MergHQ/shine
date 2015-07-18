#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "IShader.h"
#include <vector>
#include "PostProcessor.h"

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
	float time;
	GLuint quadvao, quadvbo, quadibo, quaduv;
	std::vector<GLuint> QuadIndices;
	CPostProcessor* postprocessor;
};

#endif