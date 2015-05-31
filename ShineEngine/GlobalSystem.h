#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#pragma once

class CRenderer;
class CMeshSystem;
class CCamera;
class CInput;
class CConsoleSystem;

struct IGlobalSystem
{
	IGlobalSystem();
	~IGlobalSystem();

	bool IsRunning;
	
	CInput* pInput;
	CRenderer* pRenderer;
	CMeshSystem* pMeshSystem;
	CConsoleSystem* pConsoleSystem;

	void Init();
	void Update(float dt);
	void Log(const char* _Format);
	CCamera* GetCamera() { return m_pCamera; }

private:
	CCamera* m_pCamera;
};

#endif