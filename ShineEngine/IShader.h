#ifndef _ISHADER_H_
#define _ISHADER_H_

#pragma once

#include <GL\glew.h>
#include "GLFW\glfw3.h"

/*
	This struct is used to create a shader.
*/

struct SShaderParams
{
	const char* name;
	int id;
	const char* v_file;
	const char* f_file;
};

/*
	The shader interface.
*/

struct IShader
{
	virtual const char* GetName() = 0;
	virtual int GetId() = 0;
	virtual bool LoadShader(const char* v_shader, const char* f_shader) = 0;
	virtual bool Reload() = 0;
	virtual GLuint GetShaderProgramme() = 0;
	virtual const char* GetVFile() = 0;
	virtual const char* GetFFile() = 0;
	virtual void Update() = 0;
};

#endif