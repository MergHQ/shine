#ifndef IMESH_H_
#define IMESH_H_

#pragma once

#include "shine.h"
#include "IShader.h"
#include "glm\glm.hpp"

/*
	This struct is used to create a mesh.
*/

struct SMeshParams
{

	const char* name;
	int id;
	std::vector<float> verts;
	glm::vec3 pos = glm::vec3(0, 0, 0);
	SShaderParams* pShader;

};

/*
	The mesh interface.
*/

struct IMesh
{
	virtual const char* GetName() = 0;
	virtual int GetId() = 0;
	virtual IShader* GetShader() = 0;
	virtual IShader* CreateShader(SShaderParams* pShaderParams) = 0;
	virtual std::vector<float> GetVerts() = 0;
	virtual glm::vec3 GetWorldPos() = 0;
	virtual void SetPos(glm::vec3 pos) = 0;
	virtual glm::vec4 GetWorldVec4() = 0;
};

#endif