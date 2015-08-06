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

	GLuint meshVao;
	GLuint meshVbo;
	GLuint meshInidcies;
	GLuint meshNormals;
	GLuint meshTexcoords;

	// IMesh
	virtual void SetPos(Vec3 pos);
	virtual void SetRotation(Vec3 axis, float rot);
	virtual string GetName() { return m_meshName; }
	virtual string GetFileName() { return m_file; }
	virtual int GetId() { return m_meshId; }
	virtual IMaterial* GetMaterial() { return m_pMaterial; }
	virtual std::vector<float> GetVerts() { return m_verticies; }
	virtual std::vector<unsigned int> GetIndicies() { return m_indiciesVector; }
	virtual std::vector<float> GetNormals() { return m_normals; }
	virtual std::vector<float> GetTexCoords() { return m_texcoords; }
	virtual Vec3 GetWorldPos() { return m_worldPos; }
	virtual Vec4 GetRotation() { return Vec4(m_worldRotAxis, m_worldRotScalar); }
	virtual Mat44 GetWorldTM(){ return m_tm; }
	virtual GLuint GetVao() { return meshVao; }
	virtual GLuint GetVbo() { return meshVbo; }
	virtual GLuint GetIbo() { return meshInidcies; }
	virtual GLuint GetNbo() { return meshNormals; }
	virtual GLuint GetTbo() { return meshTexcoords; }
	// ~IMesh

	void CreateVaosAndShit();
	void BuildTM(Vec3 pos, Vec3 axis, float rot);
protected:
private:
	string m_meshName;
	string m_file;
	const unsigned int m_slot = 0;
	int m_meshId;
	float m_worldRotScalar;
	IMaterial* m_pMaterial;
	std::vector<tinyobj::shape_t> ReadCompiledObj(std::istream* stream);
	std::vector<float> m_verticies;
	std::vector<float> m_normals;
	std::vector<float> m_texcoords;
	std::vector<unsigned int> m_indiciesVector;
	Vec3 m_worldPos;
	Vec3 m_worldRotAxis;
	Mat44 m_tm;
};
#endif