#include "GlobalSystem.h"
#include "shine.h"

IGlobalSystem::IGlobalSystem():
pRenderer(nullptr)
{
}

IGlobalSystem::~IGlobalSystem()
{
	delete pRenderer;
}

void IGlobalSystem::Init()
{
	Log("Initializing systems...");

	pRenderer = new CRenderer();
	Log("- Renderer");

	Log("We gucchi!");
}

void IGlobalSystem::Log(const char* _Format)
{
	printf("%s \n",_Format);
}



