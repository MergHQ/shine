#version 400

layout(location = 0) in vec3 vp;

out vec3 TexCoord;

uniform mat4 MVP;

void main () {
	
	gl_Position = vec4(vp, 1.0);
};

//@ // Shader split

#version 400

out vec4 frag_colour;	

vec2 GetScreenSpacePosition()
{
	return gl_FragCoord.xy/vec2(1280,720);
}

uniform sampler2D u_color;
uniform sampler2D u_godraycolor;
 
void main () {
	
	frag_colour = texture(u_color, GetScreenSpacePosition());
};