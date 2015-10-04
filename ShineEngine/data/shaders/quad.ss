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

uniform vec2 u_screenRes;

vec2 GetScreenSpacePosition()
{
	return gl_FragCoord.xy/u_screenRes;
}

uniform sampler2D u_color;
uniform sampler2D u_godraycolor;
uniform	sampler2D u_depth;
uniform sampler2D u_normal;

float compareDepths(float depth1,float depth2 ) 
{
	vec2 camerarange = vec2(0.1,1000);
	float aoCap = 1.0;
	float aoMultiplier=10000.0;
	float depthTolerance=0.000;
	float aorange = 10.0;
	float diff = sqrt( clamp(1.0-(depth1-depth2) / (aorange/(camerarange.y-camerarange.x)),0.0,1.0) );
	float ao = min(aoCap,max(0.0,depth1-depth2-depthTolerance) * aoMultiplier) * diff;
    return ao;
}

float linearizeDepth(vec2 coord)
{
	float f=1000.0;
	float n = 0.1;
	float z = (2 * n) / (f + n - texture2D( u_depth, coord ).x * (f - n));
	return z;
}

float ComputeSSAO()
{
	float final = 0;

		vec2 texCoord = GetScreenSpacePosition();

        float depth = linearizeDepth(texCoord);
        float d;
        float pw = 1.0 / u_screenRes.x;
        float ph = 1.0 / u_screenRes.y;
        float aoCap = 1.0;
        float ao = 0.0;
        float aoMultiplier=10000.0;
        float depthTolerance = 0.001;
        float aoscale=1.0;

		for(int i = 0; i < 4; i++)
		{
			d=linearizeDepth( vec2(texCoord.x+pw,texCoord.y+ph));
			ao+=compareDepths(depth,d)/aoscale;

			d=linearizeDepth( vec2(texCoord.x-pw,texCoord.y+ph));
			ao+=compareDepths(depth,d)/aoscale;

			d=linearizeDepth( vec2(texCoord.x+pw,texCoord.y-ph));
			ao+=compareDepths(depth,d)/aoscale;

			d=linearizeDepth( vec2(texCoord.x-pw,texCoord.y-ph));
			ao+=compareDepths(depth,d)/aoscale;

			pw*=2.0;
			ph*=2.0;
			aoMultiplier/=2.0;
			aoscale*=1.2;
		}

        ao/=16.0;
		final = ao;

	return final;
}
 
void main () {
	frag_colour =  vec4(vec3(1.0 - ComputeSSAO()), 1) * texture(u_color, GetScreenSpacePosition());
	//frag_colour =  texture(u_color, GetScreenSpacePosition());

};