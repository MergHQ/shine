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
	virtual std::vector<unsigned int> GetIndicies() { return m_indiciesVector; }
	virtual std::vector<float> GetNormals() { return m_normals; }
	virtual glm::vec3 GetWorldPos() { return m_worldPos; }
	virtual void SetPos(glm::vec3 pos);
	virtual void SetRotation(glm::vec3 axis, float rot);
	virtual glm::mat4 GetWorldTM(){ return m_tm; }
	virtual GLuint GetVao() { return meshVao; }
	virtual GLuint GetVbo() { return meshVbo; }
	virtual GLuint GetIbo() { return meshInidcies; }
	virtual GLuint GetNbo() { return meshNormals; }
	// ~IMesh

	void CreateVaosAndShit();
	void BuildTM(glm::vec3 pos, glm::vec3 axis, float rot);
	GLuint meshVao;	
	GLuint meshVbo;
	GLuint meshInidcies;
	GLuint meshNormals;
private:

	const char* m_meshName;
	int m_meshId;
	const unsigned int m_slot = 0;
	std::vector<float> m_verticies;
	std::vector<unsigned int> m_indiciesVector;
	std::vector<float> m_normals;
	const char* m_file;
	glm::vec3 m_worldPos;
	glm::vec3 m_worldRotAxis;
	float m_worldRot;
	IShader* m_pIShader;
	glm::mat4 m_tm;
	

};
#endif