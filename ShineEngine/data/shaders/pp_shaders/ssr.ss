#version 400
layout(location = 0) in vec3 vp;
layout(location = 2) in vec2 UV;

out vec2 oUV;
out float shp_;
uniform float shp;

uniform mat4 MVP;

void main () {
	
	gl_Position = vec4(vp, 1.0);
	oUV = UV;
	shp_ = shp;
};

//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.

#version 430

out vec4 frag_colour;

in vec2 oUV;

uniform sampler2D normaltex;
uniform sampler2D diffusetex;
uniform sampler2D depthtex;
in float shp_;


uniform mat4 ProjectionMatrix;

/*float linearizeDepth()
{
	float f = 1000.0;
	float n = 0.1;
	float z = (2 * n) / (f + n - texture2D( DepthBuffer, UV ).x * (f - n));
	return z;
}

// This is full copy pasta

vec4 SSR()
{
	vec3 reflectedColor = vec3(0.0f);
 
	vec3 normal = normalize( texture(NormalBuffer, UV) ).xyz;
 
	// Depth at current fragment
	float currDepth = linearizeDepth();
 
	// Eye position, camera is at (0, 0, 0), we look along negative z, add near plane to correct parallax
	vec3 eyePosition = normalize( vec3(0, 0, 0.1) );
	vec4 reflectionVector = ProjectionMatrix * reflect( vec4(-eyePosition, 0), vec4(normal, 0) ) ;
 
        // Call raytrace to get reflected color
	reflectedColor = RayTrace(reflectionVector.xyz, currDepth);	
 
 
	return vec4(reflectedColor, 1.0f);
}

vec3 RayTrace(in vec3 reflectionVector, in float startDepth)
{

	vec3 color = vec3(0.0f);
	float stepSize = 0.001; 
 
	float size = length(reflectionVector.xy);
	reflectionVector = normalize(reflectionVector/size);
	reflectionVector = reflectionVector * stepSize;
        
        // Current sampling position is at current fragment
	vec2 sampledPosition = UV;
        // Current depth at current fragment
	float currentDepth = startDepth;
        // The sampled depth at the current sampling position
	float sampledDepth = linearizeDepth();
 
        // Raytrace as long as in texture space of depth buffer (between 0 and 1)
	while(sampledPosition.x <= 1.0 && sampledPosition.x >= 0.0 &&
	      sampledPosition.y <= 1.0 && sampledPosition.y >= 0.0)
	{
                // Update sampling position by adding reflection vector's xy and y components
		sampledPosition = sampledPosition + reflectionVector.xy;
                // Updating depth values
		currentDepth = currentDepth + reflectionVector.z * startDepth;
		float sampledDepth = linearizeDepth();
                
                // If current depth is greater than sampled depth of depth buffer, intersection is found
		if(currentDepth > sampledDepth)
		{
                        // Delta is for stop the raytracing after the first intersection is found
                        // Not using delta will create "repeating artifacts"
			float delta = (currentDepth - sampledDepth);
			if(delta < 0.003f )
			{
				color = texture(tex, sampledPosition).rgb;
				break;
			}
		}
	}
 
	return color;
}

*/
void main () {
	frag_colour = texture(diffusetex, oUV);
};