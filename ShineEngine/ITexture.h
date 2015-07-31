#ifndef ITEXTURE_H
#define ITEXTURE_H
#pragma once

#include "shine.h"
#include "IShader.h"
#include <GL\glew.h>

enum TextureType
{
	DIFFUSE,
	SPECULAR,
	NORMAL
};

struct STextureParams
{
	TextureType m_Type;
	const char* m_file;
	const char* m_name;
	int m_id;
};

struct ITexture
{
	virtual void Load(){}
	virtual int GetTextureID() = 0;
	virtual string GetTextureName() = 0;
	virtual string GetTextureFile() = 0;
	virtual GLuint GetTextureId() = 0;
	virtual TextureType GetTextureType() = 0;
};
#endif