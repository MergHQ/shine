#include "MaterialSystem.h"
#include "Material.h"

CMaterialSystem::CMaterialSystem()
{
}

CMaterialSystem::~CMaterialSystem()
{
}

IMaterial* CMaterialSystem::GetMaterialById(int id)
{
	for (unsigned int i = 0; i < m_materialContainer.size(); i++)
		if (m_materialContainer[i]->GetMaterialId() == id)
			return m_materialContainer[i];

	return nullptr;
}

IMaterial* CMaterialSystem::GetMaterialByName(const char* name)
{
	// Bad way to iterate :(
	for (uint i = 0; i < m_materialContainer.size(); i++)
		if (m_materialContainer[i]->GetMaterialName() == name)
			return m_materialContainer[i];
	return nullptr;
}

IMaterial* CMaterialSystem::LoadMaterial(const char* file, string name)
{
	// We can reuse materials that are already loaded instead of creating new ones. Should save memory.
	for (uint i = 0; i < m_materialContainer.size(); i++)
	{
		// Checking if the filenames are equal.
		if (m_materialContainer[i]->GetMtlFIle() == file && m_materialContainer[i]->GetShapeName() == name)
		{
			return m_materialContainer[i];
		}	
	}

	CMaterial* nm = new CMaterial(file, name);

	if (nm != nullptr)
	{
		AddMaterial(nm);
		return nm;
	}

	return nullptr;
}

void CMaterialSystem::AddMaterial(CMaterial* pM)
{
	m_materialContainer.push_back(pM);
}

bool CMaterialSystem::DeleteMaterial(IMaterial* pM)
{
	for (uint i = 0; i < m_materialContainer.size(); i++)
		if (m_materialContainer[i] == pM)
		{
			m_materialContainer.erase(m_materialContainer.begin() + i);
			delete pM;
			return true;
		}
	return false;
}