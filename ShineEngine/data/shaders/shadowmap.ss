#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

uniform mat4 MVP;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
};

//@ // Shader split

#version 400

layout(location = 0) out float depth;	

void main () {
	
	depth = gl_FragCoord.z;		
};