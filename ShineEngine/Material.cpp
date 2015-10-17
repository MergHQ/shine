#include "Material.h"
#include "JsonTool.h"
#include <rapidjson\document.h>
#include <vector>
#include "Input.h"
#include "ITexture.h"
#include "Shader.h"
#include "Texture.h"
#include <map>

CMaterial::CMaterial(const char* file, string name)
{
	m_fileName = file;
	m_shapeName = name;
	ParseMtlFile();
}

CMaterial::~CMaterial()
{
	for (uint i = 0; i < m_textureContainer.size(); i++)
	{
		if (m_textureContainer[i] != nullptr)
			delete m_textureContainer[i];
	}

	delete m_pShader;
}

void CMaterial::ParseMtlFile()
{
	rapidjson::Document mtl;

	SJsonParser parser;
	string file = ASSET_ROOT_DIR + m_fileName;
	parser.file = file.c_str();
	/* This lib does some weird shit
	so you can't have a function return a
	Document var straight.
	Thats why we need to do this*/
	parser.IpVar = &mtl;
	parser.Run();

	if (mtl.IsObject())
	{
		// Handle json
		const rapidjson::Value& shapes = mtl["shapes"];
		for (rapidjson::SizeType i = 0; i < shapes.Size(); i++)
		{
			if (shapes[i]["shape"].GetString() == m_shapeName)
			{
				m_matName = shapes[i]["material_name"].GetString();

				// Params
				m_params[1] = atof(shapes[i]["roughness"].GetString());

				SShaderParams sparams;

				sparams.name = shapes[i]["shading"].GetString();
				sparams.s_file = shapes[i]["shading"].GetString();

				if (IShader* pShader = CreateShader(&sparams))
					m_pShader = static_cast<CShader*>(pShader);

				const rapidjson::Value& a = shapes[i]["textures"];

				if (a.IsArray())
				{
					for (rapidjson::SizeType j = 0; j < a.Size(); j++)
					{
						STextureParams texparams;
						const rapidjson::Value& obj = a[j];

						texparams.m_file = obj["texture"].GetString();
						texparams.m_name = obj["texname"].GetString();

						texparams.m_Type = (TextureType)atoi(obj["type"].GetString());

						CreateTexture(&texparams);

					}
				}
			}
		}
	}
}

ITexture* CMaterial::CreateTexture(STextureParams* params)
{
	if (!params->m_id)
	{
		params->m_id = rand() * 10000;
	}

	if (!params->m_file || params->m_file == "")
	{
		gSys->Log("[TEXTURESYS] Please define a path to the texture.");
		return nullptr;
	}

	if (!params->m_name || params->m_name == "")
	{
		gSys->Log("[TEXTURESYS] Name your texture.");
		return nullptr;
	}

	CTexture* pNtex = new CTexture(params);
	m_textureContainer.push_back(pNtex);

	return pNtex;
}

IShader* CMaterial::CreateShader(SShaderParams* params)
{
	if (params->id == 0 || !params->id)
	{
		params->id = rand() * 1000;
	}

	if (params->name == "" || !params->name)
	{
		gSys->Log("[SHADERSYS] You didn't name your shader!");
		return nullptr;
	}

	if (params->s_file == "" || !params->s_file)
	{
		gSys->Log("[SHADERSYS] No frag shader path specified!");
		return nullptr;
	}

	CShader* pNShader = new CShader(params);

	return pNShader;
}