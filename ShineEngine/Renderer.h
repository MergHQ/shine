#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once

#include <vector>
#include <GL\glew.h>
#include "GLFW\glfw3.h"

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	bool CreateMesh(float a[], int elements, const char* v_shader, const char* f_shader);
	void Render();
	std::vector<GLuint> GetVaos() { return vaos; };
	std::vector<GLuint> GetShaders() { return shaders; }

private:
	std::vector<GLuint> vaos;
	std::vector<GLuint> shaders;
};

#endif