#include "Light.h"

CLight::CLight(SLight* props)
{
	pos = props->position;
	color = props->color;
	type = props->type;
	if (type == POINTLIGHT)
		attenuation = props->attenuation;
	shadow = props->castShadows;
}

