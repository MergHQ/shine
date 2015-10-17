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

struct Shape
{	
	string name;
	IMaterial* pMaterial;
	std::vector<unsigned int> indices;

	GLuint meshVao;
	GLuint meshVbo;
	GLuint meshInidcies;
	GLuint meshNormals;
	GLuint meshTexcoords;
};


/*
The mesh interface.
*/

struct IMesh
{
	virtual ~IMesh(){}
	virtual void SetPos(Vec3 pos) = 0;
	virtual void SetRotation(Vec3 axis, float rot) = 0; //TODO: Make them QUATS!!!
	virtual string GetName() = 0;
	virtual string GetFileName() = 0;
	virtual int GetId() = 0;
	virtual Vec3 GetWorldPos() = 0;
	virtual Vec4 GetRotation() = 0;
	virtual glm::mat4& GetWorldTM() = 0;
	virtual std::vector<Shape*>& GetShapeContainer() = 0;
};

#endif