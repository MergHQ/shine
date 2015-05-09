#ifndef IMESH_H_
#define IMESH_H_

#pragma once

#include "shine.h"
#include "IShader.h"
#include "glm\glm.hpp"

/*
	This struct is used to create a mesh.
*/

struct SShineMesh
{

	const char* name;
	int id;
	std::vector<float> verts;
	glm::vec3 pos;
	IShader* pShader;

};

/*
	The mesh interface.
*/

struct IMesh
{
	virtual const char* GetName();
	virtual int GetId();
	virtual IShader* GetShader();
	virtual std::vector<float> GetVerts();
	virtual glm::vec3 GetWorldPos();
	virtual void SetPos(glm::vec3 pos);
};

#endif