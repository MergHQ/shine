#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec2 UV;
out vec3 fNormal;
out float shp_;
out vec4 fPosition;
out vec3 CamPos;

uniform mat4 MVP;
uniform mat4 Obj2World;
uniform vec3 CamPosW;
uniform float shp;
uniform mat3 normal_matrix;
uniform mat4 ViewMatrix;
uniform int lightAmmount;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	UV = vUV;
	// Convert surface normal pos to worldspace.
	fNormal = normalize(vec4(np, 0.0)).xyz;
	fPosition = Obj2World * vec4(vp, 1.0);
};

//@ // Shader split

#version 400

in vec2 UV;
in vec3 fNormal;
in vec3 CamPos;
in float shp_;
in vec4 fPosition;

layout(location = 0) out vec4 frag_colour;	
layout(location = 1) out vec3 attachNormal;
layout(location = 2) out vec3 position;

uniform sampler2D texsamp;
uniform int textures;

void main () {
	
	attachNormal = fNormal;
	position = fPosition.xyz;
	
	frag_colour = texture(texsamp, UV);	
	
};