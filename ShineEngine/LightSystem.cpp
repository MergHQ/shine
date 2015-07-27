#include "shine.h"
#include "LightSystem.h"

CLightSystem::~CLightSystem()
{
	for (uint c = 0; c < lightContainer.size();/*SEE WHAT I DID HERE :^) --->*/ c++)
	{
		if (lightContainer[c] != nullptr)
			delete lightContainer[c];
	}
}

ILight* CLightSystem::CreateLight(SLight* light)
{
	CLight* pNewlight = new CLight(light);

	if (pNewlight != nullptr)
	{
		lightContainer.push_back(pNewlight);
		return pNewlight;
	}

	return nullptr;
}


