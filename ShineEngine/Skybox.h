#ifndef SKYBOX_H
#define SKYBOX_H
#pragma once

#include "IShader.h"
#include "ITexture.h"
#include "Texture.h"
#include "FPCamera.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

class CSkyBox
{
public:
	CSkyBox();
	~CSkyBox();

	void Draw();
	CFPCamera* cam;
	CCubeMapTexture* tex;
protected:
private:
	GLuint m_textureId;
	GLuint m_vao;
	IShader* pShader;
	std::vector<float> m_verticies;
	std::vector<float> m_normals;
	std::vector<float> m_texcoords;
	std::vector<unsigned int> m_indiciesVector;

};

#endif