#ifndef SHADOWGEN_H
#define SHADOWGEN_H
#pragma once

#include "shine.h"
#include "IShader.h"

#include <glm\common.hpp>
#include <GLFW\glfw3.h>
#include <vector>



//class CShadowGen
//{
//public:
//	CShadowGen();
//	~CShadowGen();
//
//	void CreateFBO();
//	GLuint DrawDirLightShadowMap(Vec3 pos, Mat44 pm, Vec3 lookat);
//
//protected:
//private:
//
//
//};

class CShadowMapFBO
{
public:
	CShadowMapFBO(int width, int height, string shader);
	~CShadowMapFBO();

	GLuint GetTextureBufferID() { return textureid; }
	int GetTextureHeight() { return textureHeight; }
	int GetTextureWidth() { return textureWidth; }
	GLuint GetFBO() { return fbo; }
	IShader* GetShader() { return pShader; }
protected:
private:
	int textureWidth, textureHeight;
	GLuint textureid;
	GLuint fbo;
	GLuint temptex;
	IShader* pShader;

};


#endif