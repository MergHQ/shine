#include "Environment.h"

CEnvironment::CEnvironment()
{
	// Generate lut
	m_pLUT = new CLookUpTexture;

	m_pSkyBox = new CSkyBox;
}

CEnvironment::~CEnvironment()
{
	for (CIlluminationProbe* thisProbe : m_probes)
		delete thisProbe;

	delete m_pLUT;
	delete m_pSkyBox;
}

CSkyBox * CEnvironment::GetSkyBox()
{
	if (m_pSkyBox == nullptr)
	{
		m_pSkyBox = new CSkyBox;
	}
	return m_pSkyBox;
}

CIlluminationProbe* CEnvironment::CreateLightProbe(std::vector<string>& filename, Vec3 position)
{
	CIlluminationProbe* m_pProbe = new CIlluminationProbe(filename, position);
	m_probes.push_back(m_pProbe);
	return m_pProbe;
}

