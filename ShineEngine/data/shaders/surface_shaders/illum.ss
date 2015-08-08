#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec2 UV;
out vec3 fNormal;
out vec4 fPosition;
out vec4 ShadowCoord;

uniform mat4 MVP;
uniform mat4 Obj2World;
uniform mat4 DepthBias;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	UV = vUV;
	// Convert surface normal pos to worldspace.
	fNormal = normalize(Obj2World * vec4(np, 0.0)).xyz;
	fPosition = Obj2World * vec4(vp, 1.0);
	ShadowCoord = DepthBias * vec4(vp, 1.0);
};

//@ // Shader split

#version 400

in vec2 UV;
in vec3 fNormal;
in vec4 fPosition;
in vec4 ShadowCoord;


layout(location = 0) out vec4 frag_colour;	
layout(location = 1) out vec3 attachNormal;
layout(location = 2) out vec3 position;

uniform sampler2D texsamp;
uniform int textures;
uniform sampler2D shadowmap;

void main () {
	
	attachNormal = fNormal;
	position = fPosition.xyz;
	
	float visib = 1.0;
	if ( texture(shadowmap, ShadowCoord.xy ).z  <  ShadowCoord.z)
	{
		visib = 0.5;
	}
	
	if(textures == 1)
	{
		frag_colour = texture(texsamp, UV);	
	}else{
		frag_colour = vec4(1);
	}
};