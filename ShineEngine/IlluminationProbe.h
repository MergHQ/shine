#ifndef _ILLUMINATIONPROBE_H_
#define _ILLUMINATIONPROBE_H_
#pragma once

#include <vector>
#include "shine.h"
#include <freeimg\imageloader.hpp>

class CIlluminationProbe
{
public:
	CIlluminationProbe(std::vector<string>& filename, Mat44& out1, Mat44& out2, Mat44& out3);
	~CIlluminationProbe() {};

private:


	void GetTexelAttrib(const int texId, const float u, const float v, const float texelSize,
		Vec3 *direction, float *solidAngle);

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




#endif