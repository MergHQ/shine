#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H
#pragma once

#include "shine.h"
#include "ILight.h"
#include <vector>
#include "Light.h"

class CLightSystem
{
public:
	CLightSystem(){};
	~CLightSystem();

	// If you want your light to be auto garbage collected and rendered, call this function instead of just new CLight();
	ILight* CreateLight(SLight* light);
	std::vector<CLight*> lightContainer;
	std::vector<Vec3> colors;
	std::vector<Vec3> atts;
	std::vector<Vec3> positions;
	std::vector<int> types;
	std::vector<int> shadows;

	void ProcessLights();
	float GetLightRadius(ILight* light);

private:
	
};


#endif