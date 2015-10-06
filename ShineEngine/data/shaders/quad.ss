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
uniform sampler2D u_albedo;
uniform sampler2D u_godraycolor;
uniform	sampler2D u_depth;
uniform sampler2D u_normaltex;
uniform sampler2D u_positiontex;

// SSAO
	float compareDepths(float depth1,float depth2 ) 
	{
		vec2 camerarange = vec2(0.1,1000);
		float aoCap = 1.0;
		float aoMultiplier=100000.0;
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
			float pw = 1 / u_screenRes.x;
			float ph = 1 / u_screenRes.y;
			float aoCap = 1.0;
			float ao = 0.0;
			float aoMultiplier=10000.0;
			float depthTolerance = 0.001;
			float aoscale=0.9;

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
// ~SSAO

// SUN

	uniform vec3 u_CamPos;
	uniform sampler2D u_materialParams;

	// Sun constants (for now)
	const vec3 sunPos = vec3(500,200,500);
	const vec3 u_lightColor = vec3(10);

	vec4 ComputeSun()
	{
		vec4 final = vec4(0);

		vec2 sspos = GetScreenSpacePosition();
		vec3 normal = texture(u_normaltex, sspos).xyz;
		vec3 position = texture(u_positiontex, sspos).xyz;
		vec3 surfaceToCamera = normalize(u_CamPos - position);

		float NdotV = max(dot(normalize(normal), surfaceToCamera), 0.0);
		float shininess = texture(u_materialParams, sspos).x;
		float Ks = 1;
		float cook = 0;

		vec3 Ln = normalize(sunPos - position);
		vec3 H = normalize(normalize(surfaceToCamera+Ln));
                               
		float NdotH = max(dot(normalize(normal), H), 0.0);
		float NdotL = max(dot(normalize(normal), Ln), 0.0);
		float VdotH = max(dot(surfaceToCamera, H), 0.0);
                               
		// Diffuse
		vec4 diffuse = vec4(u_lightColor * (NdotL * shininess), 1);
                               
		// Geometric attenuation
		float NH2 = 2.0 * NdotH;
		float g1 = (NH2 * NdotV) / VdotH;
		float g2 = (NH2 * NdotL) / VdotH;
		float geoAtt = min(1.0, min(g1, g2));
                               
		// Roughness
		float mSquared = shininess*shininess;
		float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
		float roughness;
		if (NdotL > 0 && NdotV > 0) 
			float roughness = r1 * exp(r2);
                               
		// Fresnel
		float fresnel = pow(1.0 - VdotH, 5.0);
		fresnel *= (1.0 - Ks);
		fresnel += Ks;
								
		cook = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
		final += vec4((u_lightColor * NdotL * (0.3 + cook * (1.0-0.2))) ,1)+vec4(0.1);

		 return final;
	}

//
 
void main () {
	vec2 sspos = GetScreenSpacePosition();
	frag_colour = texture(u_albedo, sspos) + ( texture(u_color, sspos) * ComputeSun()) * vec4(vec3(1.0 - ComputeSSAO()), 1);
	//frag_colour = texture(u_albedo, sspos) * vec4(vec3(1.0 - ComputeSSAO()), 1);
	//frag_colour =  vec4(vec3(1.0 - ComputeSSAO()), 1);
};