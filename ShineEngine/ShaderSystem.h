#ifndef _ISHINESHADER_H_
#define _ISHINESHADER_H_

#include "shine.h"
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GL\GL.h>

class CShaderSystem
{
public:
	CShaderSystem();
	~CShaderSystem();

	bool LoadShader(const char* v_filePath, const char* f_filePath);
	bool Reload();
	const char* GetFShader();
	const char* GetVShader();
	GLuint s_programme;
private: 
	const char* v_shader;
	const char* f_shader;

};


#endif