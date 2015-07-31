#version 400
layout(location = 0) in vec3 vp;
uniform mat4 MVP;

void main () {
  gl_Position = MVP * vec4 (vp , 1.0);
};

//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER. VERTEX FIRST AND FRAG AFTER.

#version 430
out vec4 frag_colour;
void main () {
    frag_colour = vec4(max(pow(gl_FragCoord.w, 0.8), 0.4), max(pow(gl_FragCoord.w, 0.458), 0.5) , 1.0 , 1.0f);
};