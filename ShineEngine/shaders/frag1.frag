#version 430
out vec4 frag_colour;
void main () {
    frag_colour = vec4(max(pow(gl_FragCoord.w, 0.8), 0.2), max(pow(gl_FragCoord.w, 0.458), 0.5) , max(pow(gl_FragCoord.w, 0.4), 0.9) , 1.0f);
};