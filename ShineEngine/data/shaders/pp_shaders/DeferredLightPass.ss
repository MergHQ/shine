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
uniform vec3 lightAttenuations[20];
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
		vec4 lighting = vec4(0);
		
		vec2 sspos = GetScreenSpacePosition();
		vec3 normal = texture(normaltex, sspos).xyz;
		vec3 position = texture(positiontex, sspos).xyz;
		vec3 surfaceToCamera = normalize(CamPos - position);
		vec4 surfaceColor = texture(diffusetex, sspos);
		
		for(int i = 0; i < 20; i++)
		{
			if(i < lightAmount)
			{
				vec3 surfaceToLight = normalize(lightPositions[i] - position);
				
				// Diffuse
				float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
				vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * lightColors[i];
				
				// Specular
				float materialShininess = 10.0;
				float specularCoefficient = 0.0;
				if(diffuseCoefficient > 0)
					specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
				vec3 specular = specularCoefficient * surfaceColor.rgb * lightColors[i];
				
				// Attenuation
				float distanceToLight = length(lightPositions[i] - position);
				vec3 att = lightAttenuations[i];
				float attenuation = att.x + (att.y * distanceToLight) + (att.z * distanceToLight * distanceToLight);
				
				if(attenuation > 0)
					lighting +=vec4((diffuse + specular) / attenuation, 1.0);
				else
					lighting += vec4(diffuse + specular, 1.0);
			}
		}
		
		// Ambient
		float ambientCoefficient = 0.25;
		vec4 ambient = vec4(ambientCoefficient * surfaceColor.rgb, 1.0);
		
		frag_colour = lighting + ambient;
};