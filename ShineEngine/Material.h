#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include "IMaterial.h"
#include <vector>

class CMaterial : public IMaterial
{
public:
	CMaterial(const char* file);
	~CMaterial();

	virtual void Load(const char* file){}
	virtual const char* GetMtlFIle() { return m_fileName; }
	virtual int GetMaterialId() { return m_id; }
	virtual const char* GetMaterialName() { return m_matName; }
	virtual std::vector<ITexture*> GetTextures() { return m_textureContainer; };
	
	void ParseMtlFile();

private:
	const char* m_fileName;
	const char* m_matName;
	int m_id;
	std::vector<ITexture*> m_textureContainer;
	ITexture* m_pTexture;

};

#endif