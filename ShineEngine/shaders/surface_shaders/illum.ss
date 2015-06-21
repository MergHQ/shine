#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec2 UV;
out vec3 oNp;
out vec3 v_p;
out vec3 CamWPos;
out float shp_;
out vec3 vs_light;

uniform mat4 MVP;
uniform mat4 Obj2World;
uniform vec3 CamPosW;
uniform float shp;
uniform mat3 normal_matrix;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	v_p = (Obj2World * vec4(vp, 1.0)).xyz;
	UV = vUV;
	
	// Convert surface normal pos to worldspace.
	oNp = (Obj2World * vec4(np, 1.0)).xyz;
	CamWPos = CamPosW;
	shp_ = shp;
	
	vec4 eyeCoord = Obj2World * vec4(vp, 1.0);
	
	// Put light in eye space
	vec4 lightPos = Obj2World * vec4(10,1,10, 1);

	// Light vector
	vec3 s = normalize(vec3(lightPos - eyeCoord));
	vs_light = vec3(0.12, 0.12, 0.12) * max(dot(s, oNp), 0.0);
};

//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.

#version 430

in vec2 UV;
in vec3 oNp;
in vec3 v_p;
in vec3 CamWPos;
in float shp_;
in vec3 vs_light;

out vec4 frag_colour;
uniform sampler2D texsamp;

void main () {
	
	frag_colour = vec4(vs_light, 1) * vec4(1,1,1,1);/*texture(texsamp, vec2(UV.x, 1.0 - UV.y)); /* vec4(vec3(fresnel,fresnel,fresnel), 1.0)*/
};