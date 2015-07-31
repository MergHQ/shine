#ifndef ENV_LOADER_H
#define ENV_LOADER_H
#pragma once

#include "shine.h"

class CEnvironmentLoader
{
public:
	CEnvironmentLoader();
	~CEnvironmentLoader(){};

	void LoadLevel(string level);
	string CurrentLevelName;

private:

	bool levelIsLoaded;
	void UnloadLevel();

};

#endif
