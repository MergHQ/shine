#include "GlobalSystem.h"
#include "Renderer.h"
#include "MeshSystem.h"
#include "MainWindow.h"
#include "Camera.h"
#include "FPCamera.h"
#include "Input.h"
#include "ConsoleSystem.h"

IGlobalSystem::IGlobalSystem() :
pInput(nullptr),
pRenderer(nullptr),
pMeshSystem(nullptr),
pConsoleSystem(nullptr),
m_pCamera(nullptr)
{
}

IGlobalSystem::~IGlobalSystem()
{
	delete pInput;
	delete pRenderer;
	delete pMeshSystem;
	delete pConsoleSystem;
	delete m_pCamera;
}

void IGlobalSystem::Init()
{
	Log("Initializing systems...");

	pInput = new CInput;
	Log("- Input");
	pRenderer = new CRenderer;
	Log("- Renderer");
	pMeshSystem = new CMeshSystem;
	Log("- Mesh system");
	pConsoleSystem = new CConsoleSystem;
	Log("- Console system");
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




