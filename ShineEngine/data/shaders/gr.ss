#version 400
layout(location = 0) in vec3 position;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
}

//@ //dsd

#version 400
		
out vec4 color;

uniform int asd;

void main()
{
	color = vec4(1,0.5,0,1);
}