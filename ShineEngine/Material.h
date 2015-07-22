#ifndef MATERIAL_H
#define MATERIAL_H
#pragma once

#include "IMaterial.h"
#include "Shader.h"
#include "IShader.h"
#include <vector>

class CMaterial : public IMaterial
{
public:
	CMaterial(const char* file);
	~CMaterial();

	virtual void Load(const char* file){}
	virtual int GetMaterialId() { return m_id; }
	virtual const char* GetMtlFIle() { return m_fileName; }
	virtual const char* GetMaterialName() { return m_matName; }
	virtual std::vector<ITexture*> GetTextures() { return m_textureContainer; }
	virtual IShader* GetShader() { return m_pShader; }
	
	void ParseMtlFile();
private:
	const char* m_fileName;
	const char* m_matName;
	int m_id;
	ITexture* m_pTexture;
	CShader* m_pShader;
	std::vector<ITexture*> m_textureContainer;

	ITexture* CreateTexture(STextureParams* params);
	IShader* CreateShader(SShaderParams* params);
};
#endif