#version 400
layout(location = 0) in vec3 vp;
layout(location = 2) in vec2 UV;

out vec2 oUV;
out float shp_;
out vec3 LightPos;
uniform float shp;

uniform mat4 MVP;

void main () {
	
	gl_Position = vec4(vp, 1.0);
	LightPos = vec3(5,5,5);
	oUV = UV;
};

//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.

#version 430

out vec4 frag_colour;

in vec2 oUV;
in vec3 LightPos;

uniform sampler2D normaltex;
uniform sampler2D diffusetex;
uniform sampler2D depthtex;
uniform sampler2D positiontex;
in float shp_;

uniform mat4 ProjectionMatrix;

void main () {

	
	frag_colour = texture(diffusetex, oUV);
};