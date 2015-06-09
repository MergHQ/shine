#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#define BUFFER_OFFSET(i) ((void*)(i))

class CRenderer
{

public:
	CRenderer();
	~CRenderer();
	void Render(GLFWwindow* pWin);
	GLFWwindow* GetWin() { return window; }
private:
	GLFWwindow* window;
	float time;
};

#endif