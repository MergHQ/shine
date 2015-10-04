// Implements IShader.
#include "Shader.h"
#include "shine.h"
#include "Tools.h"
#include "JsonTool.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <rapidjson\document.h>
#include <io.h>

DWORD WINAPI listen(LPVOID lpParam)
{
	HANDLE fileChangeHandle = FindFirstChangeNotification(L"data/shaders/", true, 0x00000010);

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
	m_sfile = pShaderParams->s_file;
	LoadShader(m_sfile);

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
		LoadShader(m_sfile);
		return true;
	}
	return false;
}

bool CShader::LoadShader(string shader)
{
	// Load shaders from files.

	// Check if a shader is already loaded on this instance.

	if (!m_firstTime)
	{
		glDeleteProgram(sprog);
	}

	string file = ASSET_ROOT_DIR + m_sfile;

	std::ifstream shader_stream(file.c_str());

	std::string shadercont = "";

	if (!shader_stream.is_open())
	{
		return false;
	}

	std::string line = "";
	while (!shader_stream.eof())
	{
		std::getline(shader_stream, line);
		shadercont.append(line + "\n");
	}

	line = "";

	shader_stream.close();
	
	std::vector<std::string> shaders = SSplitString::Split(shadercont, '@');
	const char* vertex_content = shaders[0].c_str();
	const char* fragment_content = shaders[1].c_str();

	//Create shaders.

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_content, NULL);
		glCompileShader(vs);
		GLint s;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &s);
		if (!s)
		{
			printf("Error compiling vert shader %s", m_sfile.c_str());
		}
	
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_content, NULL);
		glCompileShader(fs);
		glGetShaderiv( fs, GL_COMPILE_STATUS, &s);
		if (!s)
		{
			printf("Error compiling frag shader %s", m_sfile.c_str());
		}

		GLuint shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);
		glGetProgramiv(shader_programme, GL_LINK_STATUS, &s);

		if (s == GL_FALSE)
		{
			GLint maxSize;
			glGetProgramiv(shader_programme, GL_INFO_LOG_LENGTH, &maxSize);

			std::vector<GLchar> log(maxSize);
			glGetProgramInfoLog(shader_programme, maxSize, &maxSize, &log[0]);
			gSys->Log("\n\n ==============SHADER LOG==============");
			printf(log.data());
			gSys->Log("\n\n ======================================");
		}

		if (!s)
		{
			printf("Error creating shader prog %s", m_sfile.c_str());
		}

		sprog = shader_programme;

		glDeleteShader(vs);
		glDeleteShader(fs);
		m_firstTime = false;

		GenerateUniformLocations();

	return true;
}

void CShader::GenerateUniformLocations()
{
	uniformLocations[0] = glGetUniformLocation(sprog, "MVP");
	uniformLocations[1] = glGetUniformLocation(sprog, "texsamp");
	uniformLocations[2] = glGetUniformLocation(sprog, "Obj2World");
	uniformLocations[3] = glGetUniformLocation(sprog, "DepthBias");
	uniformLocations[4] = glGetUniformLocation(sprog, "shadowmap");
	uniformLocations[5] = glGetUniformLocation(sprog, "textures");
	uniformLocations[6] = glGetUniformLocation(sprog, "u_screenRes");
}