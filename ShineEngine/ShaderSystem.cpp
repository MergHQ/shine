#include "ShaderSystem.h"
#include <iostream>
#include <fstream>


CShaderSystem::CShaderSystem()
{
}

CShaderSystem::~CShaderSystem()
{
}


bool CShaderSystem::Reload()
{
	if (v_shader != nullptr && f_shader != nullptr)
	{
		LoadShader(v_shader, f_shader);
		return true;
	} else assert("You are trying to reaload when there is no shader loaded.");

	return false;
}

bool CShaderSystem::LoadShader(const char* f_shaderfile, const char* v_shaderfile)
{
	//========================================
	// Load shaders from files.
	//========================================

	// Check if a shader is already loaded on this instance.
	if (v_shader != nullptr && f_shader != nullptr)
		glDeleteShader(s_programme);


	std::ifstream vshader(v_shaderfile);
	std::ifstream fshader(f_shaderfile);

	string vshadercont;
	string fshadercont;

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

	v_shader = vshadercont.c_str();
	f_shader = fshadercont.c_str();

	//=========================================
	// Create shaders.
	//=========================================
	
	if (v_shader != "" && f_shader != "")
	{
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &v_shader, NULL);
		glCompileShader(vs);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &f_shader, NULL);
		glCompileShader(fs);

		GLuint shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);

		s_programme = shader_programme;

	} else 
		return false;

	return true;
}