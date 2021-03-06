#include "shine.h"
#include "LightSystem.h"
#include <GLFW\glfw3.h>

CLightSystem::~CLightSystem()
{
	for (uint c = 0; c < lightContainer.size();/*SEE WHAT I DID HERE :^) --->*/ c++)
	{
		if (lightContainer[c] != nullptr)
			delete lightContainer[c];
	}

	colors.clear();
	atts.clear();
	positions.clear();
	types.clear();
	shadows.clear();
}

ILight* CLightSystem::CreateLight(SLight* light)
{
	CLight* pNewlight = new CLight(light);

	if (pNewlight != nullptr)
	{
		lightContainer.push_back(pNewlight);
		ProcessLights();
		return pNewlight;
	}

	return nullptr;
}

void CLightSystem::ProcessLights()
{
	// Clear first...
	colors.clear();
	atts.clear();
	positions.clear();
	types.clear();
	shadows.clear();

	// Then add
	for (uint j = 0; j < lightContainer.size(); j++)
	{
		if (ILight* pLight = lightContainer[j])
		{

			positions.push_back(pLight->GetPos());
			colors.push_back(pLight->GetColor());
			atts.push_back(pLight->GetAttenuation());

			if (pLight->IsShadowsEnabled())
				shadows.push_back(1);
			else
				shadows.push_back(0);

			if (pLight->GetType() == POINTLIGHT)
				types.push_back(1);
			else
				types.push_back(0);
		}
	}
}

float CLightSystem::GetLightRadius(ILight * light)
{
	float MaxChannel = fmax(fmax(light->GetColor().x, light->GetColor().y), light->GetColor().z);

	float ret = (-light->GetAttenuation().x + sqrtf(light->GetAttenuation().x * light->GetAttenuation().x -
		4 * light->GetAttenuation().y * (light->GetAttenuation().y - 256 * MaxChannel * light->GetAttenuation().z)))
		/
		2 * light->GetAttenuation().y;
	return ret;
}



