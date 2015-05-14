#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#pragma once

class CRenderer;
class CMeshSystem;
class CCamera;
class CDefaultCamera;

struct IGlobalSystem
{
	IGlobalSystem();
	~IGlobalSystem();

	bool IsRunning;
	
	CRenderer* pRenderer;
	CMeshSystem* pMeshSystem;

	void Init();
	void Update();
	void Log(const char* _Format);
	CDefaultCamera* GetDefaultCamera() { return m_pDefaultCamera; }

private:
	CCamera* m_pCamera;
	CDefaultCamera* m_pDefaultCamera;
};

#endif