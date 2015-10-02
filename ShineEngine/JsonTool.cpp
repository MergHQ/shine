#include "JsonTool.h"
#include <iostream>
#include <fstream>
#include <string>
#include <rapidjson\filestream.h>
#include <rapidjson\writer.h>

void SJsonParser::Run()
{
	FILE* pFile = fopen(file, "r");
	if (pFile != nullptr)
	{
		rapidjson::FileStream s(pFile);
		IpVar->ParseStream<0>(s);
	}
}
