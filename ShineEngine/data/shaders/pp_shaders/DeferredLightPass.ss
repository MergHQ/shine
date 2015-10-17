#version 400
layout(location = 0) in vec3 vp;
 
out vec2 oUV;
out float shp_;
uniform float shp;
 
uniform mat4 MVP;
 
void main () {
       
        gl_Position = MVP * vec4(vp, 1.0);
                shp_ = shp;
};
 
//@ //.
 
#version 400
 
layout(location = 0)out vec4 frag_colour;
 
in vec2 oUV;
 
uniform sampler2D u_normaltex;
uniform sampler2D u_albedo;
uniform sampler2D u_positiontex;

uniform sampler2D u_materialParams;
 
uniform sampler2D shadowmpapos;
 
uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec3 u_lightAttenuation; 
uniform vec3 u_CamPos; 
uniform vec2 u_screenRes;
in float shp_;
 
uniform mat4 ProjectionMatrix;
 
vec2 GetScreenSpacePosition()
{
        return gl_FragCoord.xy/u_screenRes;
}

vec4 bloom(vec2 sspos)
{
	float values[9];
	values[0]=0.05;
	values[1]=0.09;
	values[2]=0.11;
	values[3]=0.15;
	values[4]=0.2;
	values[5]=0.15;
	values[6]=0.11;
	values[7]=0.09;
	values[8]=0.05;
	
	vec3 final = vec3(0);
	
	vec2 samplePos = vec2(sspos.x - 4.0 * 0.004, sspos.y);
	for(int k = 0; k < 9; k++)
	{
		final += texture(u_albedo, samplePos).xyz * values[k];
		samplePos += 0.004;
	}

	return vec4(final, 1);	
}
 
void main () {
       
                // Cook torrance lighting model.       
               
                vec4 lighting = vec4(0);
               
                vec2 sspos = GetScreenSpacePosition();
                vec3 normal = texture(u_normaltex, sspos).xyz;
                vec3 position = texture(u_positiontex, sspos).xyz;
                vec3 surfaceToCamera = normalize(u_CamPos - position);
                vec4 surfaceColor = texture(u_albedo, sspos);
				vec4 materialParams = texture(u_materialParams, sspos);
               
                float NdotV = max(dot(normalize(normal), surfaceToCamera), 0.0);
                float shininess = materialParams.x;
                float Ks = 1;
               
                float cook = 0;
                vec3 finalValue = vec3(0);
               

                vec3 Ln = normalize(u_lightPosition - position);
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
								
				float attenuation = 1;
				vec3 att = u_lightAttenuation;

				if(att.x != 0)
				{
					float distanceToLight = length(u_lightPosition - position);
					attenuation /=(att.x * 0.1) + ((att.y* 0.1) * distanceToLight) + ((att.y * 0.1)* distanceToLight * distanceToLight);
				}
                               
               cook = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 4);
               finalValue += (u_lightColor * NdotL * (0.3 + cook * (1.0-0.2)))+vec3(0.1);
				if(attenuation != 0)
					finalValue *= attenuation;

                frag_colour = surfaceColor *  vec4(finalValue, 1.0);
};