#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#pragma once

#include <string>

class CRenderer;
class CMeshSystem;
class CCamera;
class CInput;
class CConsoleSystem;
class CConsoleInput;
class CGameInput;
class CWindowInput;
class CMaterialSystem;
class CEnvironmentLoader;

struct IGlobalSystem
{
public:
	IGlobalSystem();
	~IGlobalSystem();

	bool IsRunning;
	
	CInput* pInput;
	CRenderer* pRenderer;
	CMeshSystem* pMeshSystem;
	CConsoleSystem* pConsoleSystem;
	CMaterialSystem* pMaterialSystem;
	CEnvironmentLoader* pLevelLoader;

	CConsoleInput* m_pConIp;
	CGameInput* m_pGameIp;
	CWindowInput* m_pWinIp;

	void Init();
	void Update(float dt);
	void Log(std::string log);
	CCamera* GetCamera() { return m_pCamera; }
protected:
private:
	CCamera* m_pCamera;	
};
#endif