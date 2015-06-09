#ifndef MATSYS_H
#define MATSYS_H

#pragma once

#include "IMaterial.h"
#include <vector>

class CMaterialSystem
{
public:
	CMaterialSystem();
	~CMaterialSystem();

	IMaterial* GetMaterialById(int id);
	std::vector<IMaterial*> GetMaterialContainer() { return m_materialContainer; }
	IMaterial* GetMaterialByName(const char* name);
	// Returns a pointer to the material if successful.
	IMaterial* LoadMaterial(const char* file);
	void AddMaterial(IMaterial* pM);
	// Returns true if successful.
	bool DeleteMaterial(IMaterial* pm);

private:
	std::vector<IMaterial*> m_materialContainer;

};

#endif