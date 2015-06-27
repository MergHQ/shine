#include "GlobalSystem.h"
#include "Renderer.h"
#include "MeshSystem.h"
#include "MainWindow.h"
#include "Camera.h"
#include "FPCamera.h"
#include "Input.h"
#include "ConsoleSystem.h"
#include "ConsoleInput.h"
#include "GameInput.h"
#include "WindowInput.h"
#include "MaterialSystem.h"

IGlobalSystem::IGlobalSystem() :
pInput(nullptr),
pRenderer(nullptr),
pMeshSystem(nullptr),
pConsoleSystem(nullptr),
pMaterialSystem(nullptr),
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
	delete pMaterialSystem;

	delete m_pConIp;
	delete m_pGameIp;
	delete m_pWinIp;
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
	pMaterialSystem = new CMaterialSystem;
	Log("- Material System");
	pConsoleSystem = new CConsoleSystem;
	Log("- Console system");
	m_pCamera = new CFPCamera;
	if (m_pCamera->Init())
		Log("- Camera system");
	Log("We gucchi!");


	//--------------

	m_pConIp = new CConsoleInput;
	m_pGameIp = new CGameInput;
	m_pWinIp = new CWindowInput;
	
}

void IGlobalSystem::Update(float dt)
{
	m_pCamera->Update(dt, pRenderer->GetWin());
}

void IGlobalSystem::Log(std::string log)
{
	printf("%s \n",log.c_str());
}




