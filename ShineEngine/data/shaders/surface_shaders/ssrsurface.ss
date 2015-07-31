#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec4 fPosition;
out vec2 UV;

uniform mat4 MVP;

void main () {
	
	fPosition = MVP * vec4(vp, 1.0);
	gl_Position = fPosition;
	UV = vUV;
};

//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.

#version 430

in vec4 fPosition;

out vec4 frag_colour;
in vec2 UV;

uniform sampler2D diffusetex;
uniform sampler2D depthtex;
uniform sampler2D normaltex;

void main () {
		
	vec2 ndc = (fPosition.xy / fPosition.w);
	vec2 textcoord = vec2(ndc.x, -ndc.y) / 2.0 + 0.5;
	vec4 ReflectionColor = texture(diffusetex, textcoord);
	ReflectionColor.w = 1.0;
	

	frag_colour = vec4(ReflectionColor) * 0.7;
};