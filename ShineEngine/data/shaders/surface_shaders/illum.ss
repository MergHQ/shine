#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 np;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 tangent;

out vec2 UV;
out vec3 fNormal;
out vec4 fPosition;
out vec4 ShadowCoord;
out vec3 fTangent;

uniform mat4 MVP;
uniform mat4 Obj2World;
uniform mat4 DepthBias;

void main () {
	
	gl_Position = MVP * vec4(vp, 1.0);
	UV = vUV;
	// Convert surface normal pos to worldspace.
	fNormal = normalize(Obj2World * vec4(np, 0.0)).xyz;
	fPosition = Obj2World * vec4(vp, 1.0);
	ShadowCoord = DepthBias * vec4(vp, 1.0);
	fTangent = (Obj2World * vec4(tangent, 0.0)).xyz;
};

//@ // Shader split

#version 400

in vec2 UV;
in vec3 fNormal;
in vec4 fPosition;
in vec4 ShadowCoord;
in vec3 fTangent;

layout(location = 0) out vec4 frag_colour;	
layout(location = 1) out vec3 attachNormal;
layout(location = 2) out vec3 position;
layout(location = 3) out vec4 materialParams;
layout(location = 4) out vec4 grcolor;

uniform sampler2D texsamp;
uniform int textures;
uniform sampler2D shadowmap;
uniform int asd;
uniform float u_roughness;
uniform sampler2D u_normalMap;

vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(fNormal);
    vec3 Tangent = normalize(fTangent);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(u_normalMap, UV).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main () {
	
	if(length(texture(u_normalMap, UV).xyz) == 0)
		attachNormal = fNormal;
	else
		attachNormal = CalcBumpedNormal();

	position = fPosition.xyz;
	materialParams = vec4(u_roughness,0,0,0);
	grcolor = vec4(0);
	
	float bias = 0.0001;
	float visib = 1.0;
	if ( texture(shadowmap, ShadowCoord.xy ).z  <  ShadowCoord.z-bias)
	{
		visib = 0.5;
	}
	
	if(textures == 1)
	{
		frag_colour = texture(texsamp, UV) * visib;	
	}else{
		frag_colour = vec4(1) * visib;
	}
};