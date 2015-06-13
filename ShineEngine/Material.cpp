#include "Material.h"
#include "JsonTool.h"
#include <rapidjson\document.h>
#include <vector>
#include "Input.h"


CMaterial::CMaterial(const char* file)
{
	m_fileName = file;
	ParseMtlFile();
}

CMaterial::~CMaterial()
{
}

void CMaterial::ParseMtlFile()
{
	rapidjson::Document mtl;

	SJsonParser parser;
	parser.file = m_fileName;
	/* This lib does some weird shit
	so you can't have a function return a
	Document var straight.
	Thats why we need to do this*/
	parser.IpVar = &mtl;
	parser.Run();


	if (mtl.IsObject())
	{
		// Handle info
		gSys->Log(mtl["material_name"].GetString());
		
		m_matName = mtl["material_name"].GetString();

		const rapidjson::Value& a = mtl["textures"];
		if (a.IsArray())
		{
			for (rapidjson::SizeType i = 0; i < a.Size(); i++)
			{
				const rapidjson::Value& obj = a[i];
				gSys->Log(obj["type"].GetString());
			}
		}		
	}
}
