#include "GlobalSystem.h"
#include "Renderer.h"
#include "MeshSystem.h"

IGlobalSystem::IGlobalSystem() :
pRenderer(nullptr),
pMeshSystem(nullptr)
{
}

IGlobalSystem::~IGlobalSystem()
{
	delete pRenderer;
	delete pMeshSystem;
}

void IGlobalSystem::Init()
{
	Log("Initializing systems...");

	pRenderer = new CRenderer;
	Log("- Renderer");
	pMeshSystem = new CMeshSystem;
	Log("- Mesh system");

	Log("We gucchi!");
}

void IGlobalSystem::Log(const char* _Format)
{
	printf("%s \n",_Format);
}



