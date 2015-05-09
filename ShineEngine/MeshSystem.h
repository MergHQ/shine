#ifndef _SHINEMESH_H_
#define _SHINEMESH_H_

#pragma once

#include "shine.h"
#include "ShaderSystem.h"
#include "IMesh.h"
#include "Mesh.h"

class CMeshSystem
{

public:
	CMeshSystem(){};
	~CMeshSystem();


	IMesh* GetMeshById(int id);
	IMesh* GetMeshByName(const char* name);
	IMesh* CreateMesh(SShineMesh* pMesh);
	void DeleteMesh(int id);
	std::vector<CMesh*> GetMeshContainer(){ return mesh_instances; }

private:
	std::vector<CMesh*> mesh_instances;

};

#endif