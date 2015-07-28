#include "Light.h"

CLight::CLight(SLight* props)
{
	pos = props->position;
	color = props->color;
	type = props->type;
	if (type == POINTLIGHT)
		attenuation = props->attenuation;
	else
		attenuation = Vec3(0, 0, 0);
	shadow = props->castShadows;
}

