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
	virtual int GetTextureID() { return m_texId; }
	virtual string GetTextureName() { return m_texName; }
	virtual string GetTextureFile() { return m_texFile; }
	virtual TextureType GetTextureType() { return m_texType; }
	virtual GLuint GetTextureId() { return m_texBufferId; }
	//~ITexture
protected:
private:
	string m_texName;
	string m_texFile;
	int m_texId;
	TextureType m_texType;
	GLuint m_texBufferId;
};
#endif