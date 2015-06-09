#include "Material.h"
#include "JsonTool.h"
#include <rapidjson\document.h>
#include <rapidjson\filestream.h>
#include <vector>
#include "Input.h"


CMaterial::CMaterial(const char* file)
{
	m_fileName = file;
}

CMaterial::~CMaterial()
{
}

void CMaterial::ParseMtlFile()
{
	rapidjson::Document mtl;

	SJsonParser parser;
	parser.file = m_fileName;
	parser.OpFile = &mtl; 
	/* This lib does some weird shit
	so you can't have a function return a Document var straight. 
	Thats why we need to do this*/
	parser.Run();

	if (mtl.IsObject())
	{
		// Handle info
		
		
	}

}
