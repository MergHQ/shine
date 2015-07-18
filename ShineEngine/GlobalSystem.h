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

struct IGlobalSystem
{
	IGlobalSystem();
	~IGlobalSystem();

	bool IsRunning;
	
	CInput* pInput;
	CRenderer* pRenderer;
	CMeshSystem* pMeshSystem;
	CConsoleSystem* pConsoleSystem;
	CMaterialSystem* pMaterialSystem;

	CConsoleInput* m_pConIp;
	CGameInput* m_pGameIp;
	CWindowInput* m_pWinIp;

	void Init();
	void Update(float dt);
	void Log(std::string log);
	CCamera* GetCamera() { return m_pCamera; }

private:
	CCamera* m_pCamera;	

	// Smaller systems we don't need a public pointer to.
};

#endif