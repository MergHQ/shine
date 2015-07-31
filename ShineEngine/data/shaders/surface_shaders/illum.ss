#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec2 UV;
out vec3 fNormal;
out float shp_;
out vec4 lightPosW[20];
out vec4 fPosition;
out vec3 CamPos;

uniform mat4 MVP;
uniform mat4 Obj2World;
uniform vec3 CamPosW;
uniform float shp;
uniform mat3 normal_matrix;
uniform mat4 ViewMatrix;
uniform vec3 lightPositions[20];
uniform int lightAmmount;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	UV = vUV;
	// Convert surface normal pos to worldspace.
	fNormal = normalize(normal_matrix * np);
	shp_ = shp;
	fPosition = Obj2World * vec4(vp, 1.0);
	
	int index = 0;
	for(int i = 0; i < 20; i++)
	{
		if(index <= lightAmmount)
		{
			lightPosW[i] = ViewMatrix * vec4(lightPositions[i], 1);
		}
		else
		{
			lightPosW[i] = vec4(0);
		}
		index++;
	}
	CamPos = vec4(CamPosW, 1.0).xyz;
};

//@ // Shader split

#version 400

in vec2 UV;
in vec3 fNormal;
in vec3 CamPos;
in float shp_;
in vec4 lightPosW[20];
in vec4 fPosition;

layout(location = 0) out vec4 frag_colour;	
layout(location = 1) out vec3 attachNormal;
layout(location = 2) out vec3 position;

uniform sampler2D texsamp;
uniform int textures;
uniform vec3 lightColors[20];
uniform vec3 lightAtteniuations[20];
uniform int lightAmmount;

void main () {
	
	attachNormal = normalize(fNormal);
	position = normalize(fPosition.xyz);
	
	vec4 diffuse = vec4(0);
	vec4 specular = vec4(0);
	float attFactor = 0;
	int index = 0;
	for(int i = 0; i<20; i++)
	{
		if(index <= lightAmmount)
		{
			//Diffuse
			vec3 L = normalize(lightPosW[i].xyz - fPosition.xyz);
			float diffuseFactor = clamp(dot(L, fNormal), 0.0, 1.0);
			diffuse += vec4(vec3(lightColors[i] * diffuseFactor), 1);
			
			// Specular
			vec3 VertexEyeDiff = normalize(CamPos - fPosition.xyz);
			vec3 Reflect = normalize(reflect(-L, fNormal));
			float specularFactor = dot(Reflect, VertexEyeDiff);
			
			if(specularFactor > 0)
			{
				specularFactor = pow(specularFactor, 32.0);
				specular += vec4(vec3(1,1,1)*lightColors[i]*specularFactor, 1.0);
			}
			
			vec3 att = lightAtteniuations[i];
			float dist = length(L);
			attFactor += att.x + (att.y * dist) + (att.z * dist * dist);
			
		}
		index++;
	}
	
	if(textures > 0)
	{
		frag_colour = texture(texsamp, UV) * (vec4(0,0,0.03,1) + (diffuse / attFactor));	
	}
	else
	{
		frag_colour = vec4(1,1,1,1) * (vec4(0,0,0.03,1) + (diffuse / attFactor) + (specular / attFactor));	
	}
};