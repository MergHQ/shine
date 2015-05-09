#version 400
in vec3 vp;
void main () {
  gl_Position = vec4 (vp + vec3(1.5,1.0,-1.0) , 1.0);
};