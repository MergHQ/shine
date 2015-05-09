#include "MeshSystem.h"

CMeshSystem::~CMeshSystem()
{
	mesh_instances.clear();
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

IMesh* CMeshSystem::CreateMesh(SShineMesh* pMesh)
{
	if (pMesh->id == 0)
	{
		pMesh->id = rand() * 1000; // TODO: Check that a mesh with this ID doesn't exist.
	}
	
	if (pMesh->name == "")
	{
		printf("[MESHSYS]You didn't name your mesh!");
		return nullptr;
	}

	if (pMesh->verts.empty())
	{
		printf("[MESHSYS]Please consider addaing verticies to your mesh.");
		return nullptr;
	}

	if (pMesh->pShader == nullptr)
	{
		printf("[MESHSYS]Please consider addaing a shader to your mesh.");
		return nullptr;
	}

	if (pMesh->pos == Vec3(nullptr,nullptr,nullptr))
	{
		pMesh->pos = Vec3(0, 0, 0);
		printf("[MESHSYS]No Vec3 positions vere given, moved to origin of worldspace coordinate system.");
	}

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
				delete pMesh;
			}
		}
	}
}