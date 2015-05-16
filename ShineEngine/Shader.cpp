// Implements IShader.

#include "Shader.h"
#include <Windows.h>
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "shine.h"


DWORD WINAPI listen(LPVOID lpParam)
{
	HANDLE fileChangeHandle = FindFirstChangeNotification(L"shaders/", false, 0x00000010);

	time_t lastChange = time(0);
	bool reloaded = false;

	while (true)
	{
		DWORD Wait = WaitForSingleObject(fileChangeHandle, INFINITE);
		if (Wait == WAIT_OBJECT_0)
		{
			if (!reloaded)
			{
				lastChange = time(0);
				gSys->Log("[SHADERSYS] Recompiling shader...");
				((CShader*)lpParam)->m_shouldReload = true;
			}			
		}
		FindNextChangeNotification(fileChangeHandle);
	}
}

CShader::CShader(SShaderParams* pShaderParams) :
m_firstTime(true)
{
	m_name = pShaderParams->name;
	m_id = pShaderParams->id;
	m_v_file = pShaderParams->v_file;
	m_f_file = pShaderParams->f_file;
	LoadShader(pShaderParams->v_file, pShaderParams->f_file);

#ifdef DEV_MODE
	HANDLE listeningThread = CreateThread(NULL, 0, listen, this, 0, NULL);
#endif // DEV_MODE
}

void CShader::Update()
{
	if (m_shouldReload)
	{
		m_shouldReload = false;
		Reload();
	}
}

bool CShader::Reload()
{
	if (!m_firstTime)
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

	if (!m_firstTime)
	{
		glDeleteProgram(sprog);
	}

	std::ifstream vshader(v_shader);
	std::ifstream fshader(f_shader);

	std::string vshadercont = "";
	std::string fshadercont = "";

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
	//Create shaders.
	//=========================================

	if(vertex_content != "" && fragment_content != "")
	{
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_content, NULL);
		glCompileShader(vs);
		GLint s;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &s);
		if (!s)
		{
			printf( "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, vs);
		}


		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_content, NULL);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &s);
		if (!s)
		{
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, fs);
		}

		GLuint shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);
		glGetProgramiv(shader_programme, GL_LINK_STATUS, &s);
		if (!s)
		{
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, shader_programme);
		}

		sprog = shader_programme;

		glDeleteShader(vs);
		glDeleteShader(fs);
		m_firstTime = false;

	} else return false;

	return true;
}