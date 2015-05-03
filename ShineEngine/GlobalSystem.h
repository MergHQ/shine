#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#pragma once

#include "Renderer.h"

struct IGlobalSystem
{
	IGlobalSystem();
	~IGlobalSystem();

	bool IsRunning;
	
	CRenderer* pRenderer;
	void Init();
	void Log(const char* _Format);
};

#endif