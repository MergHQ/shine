#version 430
in vec2 UV;
out vec4 frag_colour;
uniform sampler2D texsamp;

void main () {
	
	vec4 c = texture(texsamp, UV);
	frag_colour = c;
};