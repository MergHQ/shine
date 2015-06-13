#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#pragma once

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


	void Init();
	void Update(float dt);
	void Log(const char* _Format);
	CCamera* GetCamera() { return m_pCamera; }

private:
	CCamera* m_pCamera;	

	// Smaller systems we don't need a public pointer to.
	CConsoleInput* m_pConIp;
	CGameInput* m_pGameIp;
	CWindowInput* m_pWinIp;
};

#endif