#ifndef IMESH_H_
#define IMESH_H_

#pragma once

#include "shine.h"
#include "IShader.h"
#include "glm\glm.hpp"
#include <vector>
#include "IMaterial.h"

/*
	This struct is used to create a mesh.
*/

struct SMeshParams
{

	const char* name;
	int id;
	const char* fileName;
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 rotaxis = glm::vec3(0, 1, 0);
	float rotAmmount = 0;
	const char* m_materialFile;

};

/*
	The mesh interface.
*/

struct IMesh
{
	virtual ~IMesh(){}
	virtual const char* GetName() = 0;
	virtual int GetId() = 0;
	virtual IMaterial* GetMaterial() = 0;
	virtual std::vector<float> GetVerts() = 0;
	virtual std::vector<unsigned int> GetIndicies() = 0;
	virtual std::vector<float> GetNormals() = 0;
	virtual std::vector<float> GetTexCoords() = 0;
	virtual glm::vec3 GetWorldPos() = 0;
	virtual void SetPos(glm::vec3 pos) = 0;
	virtual void SetRotation(glm::vec3 axis, float rot) = 0; //TODO: Make them QUATS!!!
	virtual glm::vec4 GetRotation() = 0;
	virtual glm::mat4 GetWorldTM() = 0;
	virtual GLuint GetVao() = 0;
	virtual GLuint GetVbo() = 0;
	virtual GLuint GetIbo() = 0;
	virtual GLuint GetNbo() = 0;
	virtual GLuint GetTbo() = 0;

};

#endif