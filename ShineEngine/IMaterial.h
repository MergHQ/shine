#ifndef IMATERIAL_H
#define IMATERIAL_H
#pragma once

#include "ITexture.h"
#include "IShader.h"
#include <vector>

struct IMaterial
{
	virtual void Load(const char* file) = 0;
	virtual const char* GetMtlFIle() = 0;
	virtual const char* GetMaterialName() = 0;
	virtual int GetMaterialId() = 0;
	virtual std::vector<ITexture*> GetTextures() = 0;
	virtual IShader* GetShader() = 0;
};
#endif