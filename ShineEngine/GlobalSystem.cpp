#include "GlobalSystem.h"
#include "Renderer.h"
#include "MeshSystem.h"
#include "MainWindow.h"
#include "Camera.h"
#include "FPCamera.h"


IGlobalSystem::IGlobalSystem() :
pRenderer(nullptr),
pMeshSystem(nullptr),
m_pCamera(nullptr)
{
}

IGlobalSystem::~IGlobalSystem()
{
	delete pRenderer;
	delete pMeshSystem;
	delete m_pCamera;
}

void IGlobalSystem::Init()
{
	Log("Initializing systems...");

	pRenderer = new CRenderer;
	Log("- Renderer");
	pMeshSystem = new CMeshSystem;
	Log("- Mesh system");
	m_pCamera = new CFPCamera;
	if (m_pCamera->Init())
		Log("- Camera system");
	Log("We gucchi!");

}

void IGlobalSystem::Update(float dt)
{
	m_pCamera->Update(dt, pRenderer->GetWin());
}

void IGlobalSystem::Log(const char* _Format)
{
	printf("%s \n",_Format);
}




