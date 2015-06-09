#ifndef ITEXTURE_H
#define ITEXTURE_H

#pragma once

#include "shine.h"
#include "IShader.h"
#include <GL\glew.h>

struct ITexture
{
	enum TextureType
	{
		DIFFUSE,
		SPECULAR,
		NORMAL
	};

	virtual void Load(const char* file, TextureType t) = 0;
	virtual int GetMaterialType() = 0;
	virtual int GetTextureID() = 0;
	virtual const char* GetTextureName() = 0;
	virtual const char* GetTextureFile() = 0;
	virtual GLuint GetTextureId() = 0;
	virtual GLuint GetTbo() = 0;
	// Get shader of the material
	virtual IShader* GetIShader() = 0;
	virtual IShader* GetDiffShader() = 0;
	virtual IShader* GetSpecShader() = 0;
	virtual IShader* GetNormalShader() = 0;


};

#endif