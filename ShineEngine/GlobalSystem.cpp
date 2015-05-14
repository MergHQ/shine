#include "GlobalSystem.h"
#include "Renderer.h"
#include "MeshSystem.h"
#include "MainWindow.h"
#include "Camera.h"
#include "DefaultCamera.h"


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
	delete m_pDefaultCamera;
}

void IGlobalSystem::Init()
{
	Log("Initializing systems...");

	pRenderer = new CRenderer;
	Log("- Renderer");
	pMeshSystem = new CMeshSystem;
	Log("- Mesh system");
	m_pCamera = new CCamera;
	if (m_pCamera->Init())
		Log("- Camera system");
	Log("We gucchi!");

	m_pDefaultCamera = new CDefaultCamera;
}

void IGlobalSystem::Update()
{
	m_pCamera->Update(pRenderer->GetWin());
}

void IGlobalSystem::Log(const char* _Format)
{
	printf("%s \n",_Format);
}




