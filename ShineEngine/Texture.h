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
	virtual void ActivateTexture(GLuint activate, GLuint uniformLoc, bool cubemap);
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
	virtual void ActivateTexture(GLuint activate, GLuint uniformLoc, bool cubemap);
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

class CFboTexture : public ITexture
{
public:
	CFboTexture(GLint cbt, GLint dataType, GLint colorAttachment, int width, int height);
	~CFboTexture();

	//ITexture
	virtual void Load(GLint cbt, GLint dataType, GLint colorAttachment, int width, int height);
	virtual GLuint GetTextureBufferId() { return m_texBufferId; }
	virtual string GetTextureName() { return m_textureName; }
	// This function is not really working with cubemaps.
	virtual string GetTextureFile() { return "this is a framebuffer texture."; }
	virtual int GetTextureId() { return m_textureId; }
	virtual void ActivateTexture(GLuint activate, GLuint uniformLoc, bool cubemap);
	//~ITexture	


private:
	GLuint m_texBufferId;
	string m_textureName;
	int m_textureId;
};

#endif