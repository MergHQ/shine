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
#include "EnvironmentIO.h"
#include "Skybox.h"
#include "EditorController.h"

IGlobalSystem::IGlobalSystem() :
pInput(nullptr),
pRenderer(nullptr),
pMeshSystem(nullptr),
pConsoleSystem(nullptr),
pMaterialSystem(nullptr),
m_pCamera(nullptr),
m_pSkyBox(nullptr)
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
	delete pLevelLoader;
	delete pEditorController;

	delete m_pConIp;
	delete m_pGameIp;
	delete m_pWinIp;
	delete m_pSkyBox;
}
 
void IGlobalSystem::Init(CMainWindow* win)
{
	pMainWin = win;

	Log("*****************************************");
	Log("S H I N E    E N G I N E");
	Log("***************************************** \n\n\n\n");
	Log("Initializing systems...\n\n");

	pInput = new CInput;
	Log("- Input");
	pMeshSystem = new CMeshSystem;
	Log("- Mesh system");
	pMaterialSystem = new CMaterialSystem;
	Log("- Material System");
	pRenderer = new CRenderer;
	pRenderer->Init(win->GetWindow());
	Log("- Renderer");
	pConsoleSystem = new CConsoleSystem;
	Log("- Console system");
	m_pCamera = new CFPCamera;
	if (m_pCamera->Init())
		Log("- Camera system\n\n");
	pLevelLoader = new CEnvironmentLoader;
#ifdef DEV_MODE
	pEditorController = new CEditorController;
	pEditorController->Init();
#endif
	Log("We gucchi!");



	//--------------

	m_pConIp = new CConsoleInput;
	m_pGameIp = new CGameInput;
	m_pWinIp = new CWindowInput;

	// Environment stuff
	m_pSkyBox = new CSkyBox;
}

void IGlobalSystem::ReleaseRenderContent()
{

	if (CMeshSystem* pMeshSys = pMeshSystem)
	{
		gSys->Log("Purging shaders, meshes, textures and materials.");
		for (uint iter = 0; iter < pMeshSys->GetMeshContainer().size(); iter++)
		{
			if (pMeshSys->GetMeshContainer()[iter] != nullptr)
			{
				// Delete all the meshes and shaders from memory, so we avoid memory leaks.
				delete pMeshSys->GetMeshContainer()[iter];
			}
		}

		for (uint j = 0; j < gSys->pMaterialSystem->GetMaterialContainer().size(); j++)
			delete pMaterialSystem->GetMaterialContainer()[j];
	}

	pInput->clearListeners();

	// Not working yet
	//pRenderer->ReleaseSystems();
}

void IGlobalSystem::Update(float dt)
{
	m_pCamera->Update(dt, pRenderer->GetWin());

}

void IGlobalSystem::Log(std::string log)
{
	printf("%s \n",log.c_str());
}