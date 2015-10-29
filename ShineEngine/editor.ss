#version 400

layout (location = 0) in vec3 vp;

uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(vp,1); 
}

// @ //.

uniform vec3 color;

out vec4 fc;

void main() {
	fc = vec4(color, 1);
}