#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec2 UV;
out vec3 oNp;
out vec3 v_p;
out vec3 CamWPos;
out float shp_;

uniform mat4 MVP;
uniform mat4 Obj2World;
uniform vec3 CamPosW;
uniform float shp;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	v_p = (Obj2World * vec4(vp, 1.0)).xyz;
	UV = vUV;
	oNp = (Obj2World * vec4(np, 1.0)).xyz;
	CamWPos = CamPosW;
	shp_ = shp;
};

//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.

#version 430

in vec2 UV;
in vec3 oNp;
in vec3 v_p;
in vec3 CamWPos;
in float shp_;

out vec4 frag_colour;
uniform sampler2D texsamp;

void main () {
	
	vec3 N = normalize(oNp);
	vec3 V = normalize(CamWPos - v_p);
	float fresnel = pow(1 - dot(N, V), shp_);
	frag_colour = texture(texsamp, UV)/* * vec4(vec3(fresnel,fresnel,fresnel), 1.0)*/;
};