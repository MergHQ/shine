#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once

#include "ITexture.h"

class CTexture : public ITexture
{
public:
	CTexture(STextureParams* params);
	~CTexture();

	//ITexture
	virtual void Load();
	virtual GLuint GetTextureBufferId() { return m_texBufferId; }
	virtual string GetTextureName() { return m_texName; }
	virtual string GetTextureFile() { return m_texFile; }
	virtual int GetTextureId() { return m_texId; }
	//~ITexture	
	
	TextureType GetTextureType() { return m_texType; }
protected:
private:
	string m_texName;
	string m_texFile;
	int m_texId;
	TextureType m_texType;
	GLuint m_texBufferId;
};

class CCubeMapTexture : public ITexture
{
public:
	CCubeMapTexture(STextureParams* params);
	~CCubeMapTexture();

	//ITexture
	virtual void Load();
	virtual GLuint GetTextureBufferId() { return m_texBufferId; }
	virtual string GetTextureName() { return m_textureName; }
	// This function is not really working with cubemaps.
	virtual string GetTextureFile() { return ""; }
	virtual int GetTextureId() { return m_textureId; }
	//~ITexture	

protected:
private:
	string m_textureName;
	int m_textureId;
	// x, -x, y, -y, z, -z
	string m_textureFiles[6];
	int m_types[6];

	GLuint m_texBufferId;
};
#endif