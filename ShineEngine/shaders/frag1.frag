#version 430

in vec3 vertexPos;
in vec3 normalPos;
in float sh;

out vec4 frag_colour;

void main () {

	float fresnel = pow(1 - dot(normalPos, vertexPos), sh);
	frag_colour = vec4(vec3(fresnel), 1);
//    frag_colour = vec4(max(pow(gl_FragCoord.w, 0.8), 0.4), max(pow(gl_FragCoord.w, 0.458), 0.5) , 1.0 , 1.0f);
};
