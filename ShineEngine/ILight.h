#ifndef ILIGHT_H
#define ILIGHT_H
#pragma once

#include "shine.h"
#include <vector>

enum LightType
{
	DIRLIGHT,
	POINTLIGHT
};

struct SLight
{
	Vec3 position;
	Vec3 color;
	Vec3 attenuation;
	bool castShadows;
	LightType type;
};
 
struct ILight
{
	virtual Vec3& GetPos() = 0;
	virtual Vec3& GetColor() = 0;
	virtual Vec3& GetAttenuation() = 0;
	virtual bool IsShadowsEnabled() = 0;
	virtual void SetPos(Vec3 pos) = 0;
	virtual void SetColor(Vec3 color) = 0;
	virtual void SetAtt(Vec3 att) = 0;
	virtual int GetType() = 0;
	virtual void EnableShadows(bool b) = 0;
	
};

typedef SLight Light;

#endif