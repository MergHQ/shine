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
	const char* name;
	IShader* m_pShader;

};

struct ITexture
{


	virtual void Load(const char* file, TextureType t) = 0;
	virtual int GetTextureType() = 0;
	virtual int GetTextureID() = 0;
	virtual const char* GetTextureName() = 0;
	virtual const char* GetTextureFile() = 0;
	virtual GLuint GetTextureId() = 0;
	virtual GLuint GetTbo() = 0;
	// Get shader of the material
	virtual IShader* GetShader() = 0;


};

#endif