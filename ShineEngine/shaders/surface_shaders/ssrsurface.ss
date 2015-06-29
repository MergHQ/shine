#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;

out vec2 UV;
out vec3 oNp;
out vec3 v_p;
out vec3 CamWPos;
out float shp_;
out vec4 lightPosW;
out vec4 eyeCoord;
out mat4 PM;

uniform mat4 MVP;
uniform mat4 Obj2World;
uniform vec3 CamPosW;
uniform float shp;
uniform mat3 normal_matrix;
uniform mat4 ProjectionMatrix;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	v_p = (Obj2World * vec4(vp, 1.0)).xyz;
	UV = vUV;
	// Convert surface normal pos to worldspace.
	oNp = (Obj2World * vec4(np, 1.0)).xyz;
	CamWPos = CamPosW;
	shp_ = shp;
	eyeCoord = Obj2World * vec4(vp, 1.0);
	// Put light in eye space
	lightPosW = Obj2World * vec4(10 + shp * 7 ,1, 10, 1);
	PM = ProjectionMatrix;
};

//@ // THIS CHAR IS IMPORTANT. IT SPLITS THE SHADER.

#version 430

in vec2 UV;
in vec3 oNp;
in vec3 v_p;
in vec3 CamWPos;
in float shp_;
in vec4 lightPosW;
in vec4 eyeCoord;
in mat4 PM;

out vec4 frag_colour;

uniform sampler2D diffusetex;
uniform sampler2D depthtex;
uniform sampler2D normaltex;

// This is full copy pasta
void main () {

	vec3 normal = normalize(texture(normaltex, UV)).xyz;
 
	float currDepth = (2 * 0.1) / (1000.0 + 0.1 - texture(depthtex, UV).x * (1000.0 - 0.1));
 
	vec3 eyePosition = normalize(normalize(eyeCoord.xyz) + vec3(0, 0, 0.1));
	vec4 reflectionVector = PM * reflect(vec4(-eyePosition, 0), vec4(normal, 0));
	
	float stepSize = 0.01; 
 
	float size = length(reflectionVector.xy);
	reflectionVector = normalize(reflectionVector / size);
	reflectionVector = reflectionVector * stepSize;
        
	vec2 sampledPosition = UV;
	float currentDepth = currDepth;
	float sampledDepth = (2 * 0.1) / (1000.0 + 0.1 - texture( depthtex, UV ).x * (1000.0 - 0.1));
	
	vec3 color = vec3(1,0,0);
	
	while(sampledPosition.x <= 1.0 && sampledPosition.x >= 0.0 && sampledPosition.y <= 1.0 && sampledPosition.y >= 0.0)
	{
		sampledPosition = sampledPosition + reflectionVector.xy;
		currentDepth = currentDepth + reflectionVector.z * currentDepth;
		float sampledDepth = (2 * 0.1) / (1000.0 + 0.1 - texture( depthtex, UV ).x * (1000.0 - 0.1));
                
		if(currentDepth > sampledDepth)
		{
			float delta = (currentDepth - sampledDepth);
			if(delta < 0.003 )
			{
				color = texture(diffusetex, sampledPosition).xyz;
			}
		}
	}
 
 

	frag_colour = vec4(color, 1.0);
};