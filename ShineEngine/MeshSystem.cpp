#include "MeshSystem.h"
#include "Shader.h"

CMeshSystem::~CMeshSystem()
{
	mesh_instances.clear();
	shader_instances.clear();
}

IMesh* CMeshSystem::GetMeshById(int id)
{
	for (unsigned int iter = 0; iter < mesh_instances.size(); iter++)
	{
		if (mesh_instances[iter]->GetId() == id)
		{
			return mesh_instances[iter];
		}
	}
	return nullptr;
}

IMesh* CMeshSystem::GetMeshByName(const char* name)
{
	for (uint iter = 0; iter < mesh_instances.size(); iter++)
	{
		if (mesh_instances[iter]->GetName() == name)
		{
			return mesh_instances[iter];
		}
	}
	return nullptr;
}

IMesh* CMeshSystem::CreateMesh(SMeshParams* pMesh)
{
	if (!pMesh->id)
	{
		pMesh->id = rand() * 1000; // TODO: Check that a mesh with this ID doesn't exist.
	}
	
	if (!pMesh->name)
	{
		gSys->Log("[MESHSYS]You didn't name your mesh!");
		return nullptr;
	}

	if (!pMesh->m_materialFile || pMesh->m_materialFile == "")
	{
		printf("[MESHSYS]Please consider addaing a material to your mesh.");
		return nullptr;
	}

	//pMesh->pos = Vec3(0.0f, 0.0f, 0.0f);

	CMesh* pNMesh = new CMesh(pMesh);

	// Add to the mesh container.
	mesh_instances.push_back(pNMesh);

	return pNMesh;
}

void CMeshSystem::DeleteMesh(int id)
{
	// TODO not working.
	if (IMesh* pMesh = GetMeshById(id))
	{
		// There must be a better way for this... Looping throught every mesh in the current context is pretty slow.
		for (unsigned int iter = 0; iter < mesh_instances.size(); iter++)
		{
			if (mesh_instances[iter] == pMesh)
			{
				mesh_instances.erase(mesh_instances.begin() + iter);
				shader_instances.erase(shader_instances.begin() + iter);
				delete pMesh;
				//delete shader_instances[iter];
			}
		}
	}
}