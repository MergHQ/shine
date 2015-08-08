#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec3 TexCoord;

uniform mat4 MVP;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	TexCoord = vp;
};

//@ // Shader split

#version 400

in vec3 TexCoord;

layout(location = 0) out vec4 frag_colour;	

uniform samplerCube cubemap;

void main () {
	
	frag_colour = texture(cubemap, TexCoord);		
};