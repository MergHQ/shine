#ifndef _ENV_H_
#define _ENV_H_

#include "Skybox.h"
#include "IlluminationProbe.h"
#include "shine.h"

class CEnvironment
{
public:
	CEnvironment();
	~CEnvironment();

	CLookUpTexture* GetLUT() { return m_pLUT; }
	CSkyBox* GetSkyBox();
	CIlluminationProbe* CreateLightProbe(std::vector<string>& filename, Vec3 position);
	std::vector<CIlluminationProbe*>& GetLightProbeList() { return m_probes; }

private:
	CLookUpTexture* m_pLUT;
	CSkyBox* m_pSkyBox;
	std::vector<CIlluminationProbe*> m_probes;
	
};

#endif