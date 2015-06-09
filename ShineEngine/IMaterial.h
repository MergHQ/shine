#ifndef IMATERIAL_H
#define IMATERIAL_H

#pragma once

#include <vector>
#include "ITexture.h"

struct IMaterial
{
	virtual void Load(const char* file) = 0;
	virtual const char* GetMtlFIle() = 0;
	virtual int GetMaterialID() = 0;
	virtual const char* GetMaterialName() = 0;
	virtual ITexture* GetITexture() = 0;
	virtual std::vector<ITexture*> GetTextures() = 0;

};

#endif