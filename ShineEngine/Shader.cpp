#include "Shader.h"
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include <iostream>
#include <fstream>
#include <string>

CShader::CShader(SShaderParams* pShaderParams) :
firstTime(true)
{
	m_name = pShaderParams->name;
	m_id = pShaderParams->id;
	m_v_file = pShaderParams->v_file;
	m_f_file = pShaderParams->f_file;
	LoadShader(pShaderParams->v_file, pShaderParams->f_file);
	
}

bool CShader::Reload()
{
	if (!firstTime)
	{
		LoadShader(m_v_file, m_f_file);
		return true;
	}
	return false;
}

bool CShader::LoadShader(const char* v_shader, const char* f_shader)
{
	//========================================
	// Load shaders from files.
	//========================================

	// Check if a shader is already loaded on this instance.
	if (!firstTime)
		glDeleteShader(sprogramme);

	std::ifstream vshader(v_shader);
	std::ifstream fshader(f_shader);

	std::string vshadercont;
	std::string fshadercont;

	if (!vshader.is_open() || !fshader.is_open())
		return false;

	std::string line = "";
	while (!vshader.eof())
	{
		std::getline(vshader, line);
		vshadercont.append(line + "\n");
	}

	line = "";

	while (!fshader.eof())
	{
		std::getline(fshader, line);
		fshadercont.append(line + "\n");
	}

	vshader.close();
	fshader.close();

	const char* vertex_content= vshadercont.c_str();
	const char* fragment_content = fshadercont.c_str();

	//=========================================
	// Create shaders.
	//=========================================

	if(vertex_content != "" && fragment_content != "")
	{
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_content, NULL);
		glCompileShader(vs);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_content, NULL);
		glCompileShader(fs);

		GLuint shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);

		sprogramme = shader_programme;
		firstTime = false;

	}
	else return false;

	return true;
}