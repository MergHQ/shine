#ifndef _MESH_H_
#define _MESH_H_

#pragma once
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"



class CMesh : public IMesh
{
public:
	CMesh(SMeshParams* pMesh);
	~CMesh();


	// IMesh
	virtual const char* GetName() { return m_meshName; }
	virtual int GetId() { return m_meshId; }
	virtual IShader* GetShader() { return m_pIShader; }
	virtual IShader* CreateShader(SShaderParams* pShaderParams);
	virtual std::vector<float> GetVerts() { return m_verticies; }
	virtual glm::vec3 GetWorldPos() { return m_worldPos; }
	virtual void SetPos(glm::vec3 pos);
	virtual void SetRotation(glm::vec3 axis, float rot);
	virtual glm::mat4 GetWorldTM(){ return m_tm; }
	virtual GLuint GetVao() { return meshVao; }
	// ~IMesh

	void CreateVaosAndShit();
	void BuildTM(glm::vec3 pos, glm::vec3 axis, float rot);
	GLuint meshVao;
private:

	const char* m_meshName;
	int m_meshId;
	std::vector<float> m_verticies;
	glm::vec3 m_worldPos;
	glm::vec3 m_worldRotAxis;
	float m_worldRot;
	IShader* m_pIShader;
	GLuint meshVbo;
	glm::mat4 m_tm;
	

};
#endif