#include "Renderer.h"
#include "shine.h"
#include <iostream>
#include <fstream>

CRenderer::CRenderer()
{
	//vaos.empty();
}

CRenderer::~CRenderer()
{
	vaos.empty();
}

bool CRenderer::CreateMesh(float a[], int elements, const char* v_shader, const char* f_shader)
{
	//std::string inputfile = filename;
	//std::vector<tinyobj::shape_t> verts;
	//std::vector<tinyobj::material_t> materials;

	//std::string err = tinyobj::LoadObj(verts, materials, inputfile.c_str());

	//if (!err.empty())
	//{
	//	return false;
	//}


	//========================================
	// Create VBOs and VAOs
	//========================================

	std::ifstream vshader(v_shader);
	std::ifstream fshader(f_shader);

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

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * elements, a, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	vaos.push_back(vao);

	//=========================================
	// Create shaders
	//=========================================

	const char* v = vshadercont.c_str();
	const char* f = fshadercont.c_str();

	if (v != "" && f != "")
	{
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &v, NULL);
		glCompileShader(vs);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &f, NULL);
		glCompileShader(fs);

		GLuint shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);

		shaders.push_back(shader_programme);
	}
	else
	{ 
		//gSys->Log("Error loading shader. Return value is null"); 
	}

	return true;
}

void CRenderer::Render()
{
	// Render yo shit
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	rot++;
	if (this != nullptr)
	{
		for (int iter = 0; iter < this->GetVaos().size(); iter++)
		{			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			glUseProgram(this->GetShaders()[iter]);
			glBindVertexArray(this->GetVaos()[iter]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
}

