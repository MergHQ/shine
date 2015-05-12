#version 430
out vec4 frag_colour;
void main () {
    frag_colour = vec4(max(pow(gl_FragCoord.w, 0.4), 0.2), 0 , 0.0f , 1.0f);
};