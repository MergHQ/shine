#ifndef _SHINEMESH_H_
#define _SHINEMESH_H_
#pragma once

#include "shine.h"
#include "IMesh.h"
#include "Mesh.h"
#include "Shader.h"

class CMeshSystem
{
public:
	CMeshSystem(){};
	~CMeshSystem();

	IMesh* GetMeshById(int id);
	IMesh* GetMeshByName(const char* name);
	IMesh* CreateMesh(SMeshParams* pMesh);
	std::vector<CMesh*>& GetMeshContainer(){ return mesh_instances; }
	std::vector<CShader*> GetShaderConteainer(){ return shader_instances; }
	void AddToShaderContainer(CShader* pShader) { shader_instances.push_back(pShader); }
	void DeleteMesh(int id);
protected:
private:
	std::vector<CMesh*> mesh_instances;
	std::vector<CShader*> shader_instances;
};
#endif