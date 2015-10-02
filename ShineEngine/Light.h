#ifndef LIGHT_H
#define LIGHT_H
#pragma once

#include "ILight.h"
#include "shine.h"

class CLight : public ILight
{
public:
	CLight(SLight* asd);
	~CLight(){};

	//ILight
	virtual Vec3& GetPos() { return pos; }
	virtual Vec3& GetColor() { return color; }
	virtual Vec3& GetAttenuation() { return attenuation; }
	virtual bool IsShadowsEnabled() { return shadow; }
	virtual void SetPos(Vec3 newpos) { pos = newpos; }
	virtual void SetColor(Vec3 newcolor){ color = newcolor; }
	virtual void SetAtt(Vec3 newatt){ attenuation = newatt; }
	virtual int GetType() { return type; }
	virtual void EnableShadows(bool b){ shadow = b; }
	//~ILight

private:
	Vec3 pos;
	Vec3 color;
	Vec3 attenuation;
	bool shadow;
	LightType type;

};


#endif