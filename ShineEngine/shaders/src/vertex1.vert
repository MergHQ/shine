#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;

out vec3 normalPos;
out vec3 vertexPos;
out float sh;
out mat4 outmvp;

uniform mat4 MVP;
uniform float shp;

void main () {
  gl_Position = MVP * vec4 (vp , 1.0);
  vertexPos = vp;
  normalPos = np;
  sh = shp;
  outmvp = MVP;
};
