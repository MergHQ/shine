#ifndef _ILLUMINATIONPROBE_H_
#define _ILLUMINATIONPROBE_H_
#pragma once

#include <vector>
#include "shine.h"
#include <freeimg\imageloader.hpp>
#include "ITexture.h"

struct IrradianceValues
{
	Mat44 m_red, m_blue, m_green;
};

class CIlluminationProbe
{
public:
	// File name is optional if you want the cubemap to be loaded from a file.
	CIlluminationProbe(std::vector<string>& filename, Vec3 position);
	~CIlluminationProbe();

	IrradianceValues* GetIrradianceValues() { return m_values; }
	ITexture* GetCubeMapTex() { return m_pCmTex; }

private:
	IrradianceValues* m_values;
	void GenerateImageData(Vec3 position, BYTE& imageData);
	void GetTexelAttrib(const int texId, const float u, const float v, const float texelSize, Vec3 *direction, float *solidAngle);
	ITexture* m_pCmTex;
};

class CEnvTexture
{
public:
	CEnvTexture(std::vector<string>& file);
	~CEnvTexture();

	std::vector<GLubyte*> data;
	std::vector<float> size;
	

private:
	std::vector<Image_t*>shits;

};

class CLookUpTexture
{
public:
	CLookUpTexture();
	~CLookUpTexture();

	GLuint& GetTextureID() { return m_texId; };
private:
	float GSmith(float roughness, float ndotv, float ndotl);
	uint32_t ReverseBits(uint32_t v);
	GLuint m_texId;

};



#endif