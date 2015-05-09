#ifndef _MESH_H_
#define _MESH_H_

#pragma once
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"



class CMesh : public IMesh
{
public:
	CMesh(SShineMesh* pMesh);
	~CMesh();


	// IMesh
	virtual int GetId() { return m_meshId; }
	virtual const char* GetName() { return m_meshName; }
	virtual std::vector<float> GetVerts() { return m_verticies; }
	virtual glm::vec3 GetWorldPos() { return m_worldPos; }
	virtual void SetPos(glm::vec3 pos);
	virtual IShader* GetShader() { return m_pIShader; }
	// ~IMesh

	void CreateVaosAndShit();
	GLuint meshVao;
	GLuint meshVbo;
private:
	const char* m_meshName;
	int m_meshId;
	std::vector<float> m_verticies;
	glm::vec3 m_worldPos;
	IShader* m_pIShader;

};
#endif