#version 400
layout(location = 0) in vec3 vp;
 
out vec2 oUV;
out float shp_;
uniform float shp;
 
uniform mat4 MVP;
 
void main () {
       
        gl_Position = vec4(vp, 1.0);
                shp_ = shp;
};
 
//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.
 
#version 400
 
out vec4 frag_colour;
 
in vec2 oUV;
 
uniform sampler2D normaltex;
uniform sampler2D diffusetex;
uniform sampler2D depthtex;
uniform sampler2D positiontex;
uniform sampler2D godraycolor;
 
uniform sampler2D shadowmpapos;
 
uniform vec3 lightPositions[20];
uniform vec3 lightColors[20];
uniform vec3 lightAttenuations[20];
uniform int lightAmount;

uniform vec2 lightsspos;
 
uniform vec3 CamPos;
 
uniform mat4 ViewMatrix;
uniform samplerCube envMap;
 
in float shp_;
 
uniform mat4 ProjectionMatrix;
 
vec2 GetScreenSpacePosition()
{
        return gl_FragCoord.xy/vec2(1280,720);
}

vec4 ComputeVolumetricLighting(vec2 sspos)
{
        const int SAMPLES = 128;
               
        float intensity = 0.25;
        float decay = 0.96875;
        vec2 texcoord = sspos;
        vec2 dir = (lightsspos / 1000) - texcoord;
        dir /= SAMPLES;
        vec3 godrays = texture(godraycolor, texcoord).xyz;
               
        for(int j = 0; j < SAMPLES; j++)
        {
                godrays += texture(godraycolor, texcoord).xyz * intensity;
                intensity *= decay;
                texcoord += dir;       
        }
       
        return vec4(godrays, 1.0);
}
 
void main () {
       
                // Cook torrance lighting model.       
               
                vec4 lighting = vec4(0);
               
                vec2 sspos = GetScreenSpacePosition();
                vec3 normal = texture(normaltex, sspos).xyz;
                vec3 position = texture(positiontex, sspos).xyz;
                vec3 surfaceToCamera = normalize(CamPos - position);
                vec4 surfaceColor = texture(diffusetex, sspos);
               
                float NdotV = max(dot(normalize(normal), surfaceToCamera), 0.0);
                float shininess = 0.4;
                float Ks = 0.8;
               
                float cook = 0;
                vec3 finalValue = vec3(0);
               
                for(int i = 0; i < 20; i++)
                {
                        if(i < lightAmount)
                        {
                                vec3 Ln = normalize(lightPositions[i] - position);
                                vec3 H = normalize(normalize(surfaceToCamera+Ln));
                               
                                float NdotH = max(dot(normalize(normal), H), 0.0);
                                float NdotL = max(dot(normalize(normal), Ln), 0.0);
                                float VdotH = max(dot(surfaceToCamera, H), 0.0);
                               
                                // Diffuse
                                vec4 diffuse = vec4(lightColors[i] * (NdotL * shininess), 1);
                               
                                // Geometric attenuation
                                float NH2 = 2.0 * NdotH;
                                float g1 = (NH2 * NdotV) / VdotH;
                                float g2 = (NH2 * NdotL) / VdotH;
                                float geoAtt = min(1.0, min(g1, g2));
                               
                                // Roughness
                                float mSquared = shininess*shininess;
                                float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
                                float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
                                float roughness = r1 * exp(r2);
                               
                                // Fresnel
                                float fresnel = pow(1.0 - VdotH, 5.0);
                                fresnel *= (1.0 - Ks);
                                fresnel += Ks;
                               
                                cook = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
                                finalValue += (lightColors[i] * NdotL * (0.3 + cook * (1.0-0.2))) + diffuse.xyz;
                        }
                }
				
                frag_colour = surfaceColor * (vec4(0.3,0.3,0.5,1) + vec4(finalValue, 1.0));
				//frag_colour += ComputeVolumetricLighting(sspos); //Broken after upgrading shader model.
};