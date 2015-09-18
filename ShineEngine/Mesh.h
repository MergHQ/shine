#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include "IMesh.h"
#include "IMaterial.h"
#include "Material.h"
#include "tiny_obj_loader.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

class CMesh : public IMesh
{
public:
	CMesh(SMeshParams* pMesh);
	CMesh::~CMesh();

	// IMesh
	virtual void SetPos(Vec3 pos);
	virtual void SetRotation(Vec3 axis, float rot);
	virtual string GetName() { return m_meshName; }
	virtual string GetFileName() { return m_file; }
	virtual int GetId() { return m_meshId; }
	virtual Vec3 GetWorldPos() { return m_worldPos; }
	virtual Vec4 GetRotation() { return Vec4(m_worldRotAxis, m_worldRotScalar); }
	virtual Mat44 GetWorldTM(){ return m_tm; }
	virtual std::vector<Shape*> GetShapeContainer() { return m_shapeContainer; }

	// ~IMesh
	std::vector<tinyobj::shape_t> shapes;
	void CreateBufferObjects();
	void BuildTM(Vec3 pos, Vec3 axis, float rot);
protected:
private:
	string m_meshName;
	string m_file;
	const unsigned int m_slot = 0;
	int m_meshId;
	float m_worldRotScalar;
	std::vector<tinyobj::shape_t> ReadCompiledObj(std::istream* stream);
	std::vector<Shape*> m_shapeContainer;
	Vec3 m_worldPos;
	Vec3 m_worldRotAxis;
	Mat44 m_tm;
	string m_materialFile;
};
#endif