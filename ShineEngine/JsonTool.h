#ifndef JSONTOOL_H
#define JSONTOOL_H
#pragma once

#include <rapidjson\reader.h>
#include <rapidjson\document.h>

struct SJsonParser
{
public:
	const char* file;
	rapidjson::Document* IpVar;

	void Run();
protected:
private:
};
#endif