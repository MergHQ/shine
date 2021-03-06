#ifndef MATSYS_H
#define MATSYS_H
#pragma once

#include "IMaterial.h"
#include "Material.h"
#include <vector>

class CMaterialSystem
{
public:
	CMaterialSystem();
	~CMaterialSystem();

	
	// Returns true if successful.
	bool DeleteMaterial(IMaterial* pm);
	IMaterial* GetMaterialById(int id);
	IMaterial* GetMaterialByName(const char* name);
	// Returns a pointer to the material if successful.
	IMaterial* LoadMaterial(const char* file, string name);
	std::vector<CMaterial*> GetMaterialContainer() { return m_materialContainer; }
protected:
private:
	std::vector<CMaterial*> m_materialContainer;
	void AddMaterial(CMaterial* pM);
};
#endif