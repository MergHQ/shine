/*
	HOW TO CREATE A NEW MESH:
	
	#include "IMesh.h"

	SMeshParams mymesh;
	mymesh.fileName = "pathtoobjfile";
	mymesh.m_materialFile = "pathtomaterialfile";

	gSys->pMeshSystem->CreateMesh(&mymesh);

	THATS IT.


*/

#ifndef IMESH_H_
#define IMESH_H_
#pragma once

#include "shine.h"
#include "IShader.h"
#include "IMaterial.h"
#include <vector>

/*
This struct is used to create a mesh.
*/

struct SMeshParams
{
	const char* name;
	int id;
	const char* fileName;
	Vec3 pos = Vec3(0, 0, 0);
	Vec3 rotaxis = Vec3(0, 1, 0);
	float rotAmmount = 0;
	const char* m_materialFile;
};

/*
The mesh interface.
*/

struct IMesh
{
	virtual ~IMesh(){}
	virtual void SetPos(Vec3 pos) = 0;
	virtual void SetRotation(Vec3 axis, float rot) = 0; //TODO: Make them QUATS!!!
	virtual const char* GetName() = 0;
	virtual int GetId() = 0;
	virtual IMaterial* GetMaterial() = 0;
	virtual std::vector<float> GetVerts() = 0;
	virtual std::vector<unsigned int> GetIndicies() = 0;
	virtual std::vector<float> GetNormals() = 0;
	virtual std::vector<float> GetTexCoords() = 0;
	virtual Vec3 GetWorldPos() = 0;
	virtual Vec4 GetRotation() = 0;
	virtual glm::mat4 GetWorldTM() = 0;
	virtual GLuint GetVao() = 0;
	virtual GLuint GetVbo() = 0;
	virtual GLuint GetIbo() = 0;
	virtual GLuint GetNbo() = 0;
	virtual GLuint GetTbo() = 0;
};
#endif