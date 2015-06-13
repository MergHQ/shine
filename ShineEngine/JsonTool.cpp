#include "JsonTool.h"
#include <iostream>
#include <fstream>
#include <string>
#include <rapidjson\filestream.h>

void SJsonParser::Run()
{
	FILE* pFile = fopen(file, "r");
	if (pFile != nullptr)
	{
		rapidjson::FileStream s(pFile);
		result.ParseStream<0>(s);
	}
}