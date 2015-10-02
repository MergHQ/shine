#ifndef ENV_IO_H
#define ENV_IO_H
#pragma once

#include "shine.h"

class CEnvironmentLoader
{
public:
	CEnvironmentLoader();
	~CEnvironmentLoader(){};

	void LoadLevel(string level);
	void DumpCurrentLevel();
	string CurrentLevelName;

private:

	bool levelIsLoaded;
	void UnloadLevel();

};



#endif
