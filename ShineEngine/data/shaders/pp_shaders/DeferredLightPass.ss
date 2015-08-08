#version 400
layout(location = 0) in vec3 vp;
 
out vec2 oUV;
out float shp_;
uniform float shp;
 
uniform mat4 MVP;
 
void main () {
       
        gl_Position = vec4(vp, 1.0);
};
 
//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.
 
#version 400
 
out vec4 frag_colour;
 
in vec2 oUV;
 
uniform sampler2D normaltex;
uniform sampler2D diffusetex;
uniform sampler2D depthtex;
uniform sampler2D positiontex;

uniform sampler2D shadowmpapos;
 
uniform vec3 lightPositions[20];
uniform vec3 lightColors[20];
uniform vec3 lightAtteniuations[20];
uniform int lightAmount;
 
uniform vec3 CamPos;
 
uniform mat4 ViewMatrix;
 
in float shp_;
 
uniform mat4 ProjectionMatrix;
 
vec2 GetScreenSpacePosition()
{
        return gl_FragCoord.xy/vec2(1280,720);
}
 
void main () {
 
        vec2 sspos = GetScreenSpacePosition();
       
        vec3 position = texture(positiontex, sspos).xyz;
        vec3 normal = texture(normaltex, sspos).xyz;
 
        vec4 diffuse = vec4(0);
		 	
        for(int i = 0; i < 20; i++)
        {
                if(i < lightAmount)
                {
                        //Diffuse
                        vec3 L = lightPositions[i] - position;
                        vec3 LN = normalize(L);
						vec3 att = lightAtteniuations[0];
						float dist = length(L);
						float attFactor = att.x + (att.y * dist) + (att.z * dist * dist);
						
						if(attFactor > 0)
						{
 
							float diffuseFactor = clamp(dot(LN, normal), 0.0, 1.0);
							diffuse += vec4(vec3(lightColors[i] * diffuseFactor), 1) / attFactor;
	 
							// Specular
							vec3 VertToEye = normalize(CamPos - position);
							vec3 ReflectedVector = normalize(reflect(-LN, normal));
							float specularfactor = dot(ReflectedVector, VertToEye);
						   
							if(specularfactor > 0)
							{
									specularfactor = pow(specularfactor, 10.0);
									diffuse += vec4(vec3(1) * lightColors[i] * specularfactor, 1.0) / attFactor;
							}
 
                       } else {
							
							float diffuseFactor = clamp(dot(LN, normal), 0.0, 1.0);
							diffuse += vec4(vec3(lightColors[i] * diffuseFactor), 1);
	 
							// Specular
							vec3 VertToEye = normalize(CamPos - position);
							vec3 ReflectedVector = normalize(reflect(-LN, normal));
							float specularfactor = dot(ReflectedVector, VertToEye);
						   
							if(specularfactor > 0)
							{
									specularfactor = pow(specularfactor, 10.0);
									diffuse += vec4(vec3(1) * lightColors[i] * specularfactor, 1.0);
							}
					   }
                         
                }
        }
       
       
        frag_colour = texture(diffusetex, sspos) * diffuse ;
};