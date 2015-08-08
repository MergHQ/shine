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

enum FBOTextureType
{
	DEPTH,
	COLOR,
	NORMALANDPOS,
};

struct STextureParams
{
	TextureType m_Type;
	const char* m_file;
	const char* m_name;
	int m_id;
	string cubemapTextures[6];
};

struct ITexture
{
	virtual void Load(){}
	virtual GLuint GetTextureBufferId() = 0;
	virtual string GetTextureName() = 0;
	virtual string GetTextureFile() = 0;
	virtual int GetTextureId() = 0;

};
#endif