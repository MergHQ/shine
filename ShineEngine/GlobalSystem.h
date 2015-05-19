#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#pragma once

class CRenderer;
class CMeshSystem;
class CCamera;

struct IGlobalSystem
{
	IGlobalSystem();
	~IGlobalSystem();

	bool IsRunning;
	
	CRenderer* pRenderer;
	CMeshSystem* pMeshSystem;

	void Init();
	void Update(float dt);
	void Log(const char* _Format);
	CCamera* GetCamera() { return m_pCamera; }

private:
	CCamera* m_pCamera;
};

#endif