#include "shine.h"
#include "Mesh.h"
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include <glm\gtc\matrix_transform.hpp>


CMesh::CMesh(SMeshParams* pMesh)
{
	m_meshId = pMesh->id;
	m_meshName = pMesh->name;
	m_verticies = pMesh->verts;
	CreateVaosAndShit();
	m_worldPos = pMesh->pos;
	m_worldRotAxis = pMesh->rotaxis;
	m_worldRot = pMesh->rotAmmount;
	m_pIShader = CreateShader(pMesh->pShader);
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRot);
}

CMesh::~CMesh() {}

void CMesh::SetPos(Vec3 pos)
{
	m_worldPos = pos;
}

void CMesh::SetRotation(glm::vec3 axis, float rot)
{
	m_worldRotAxis = axis;
	m_worldRot = rot;
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRot);
}

void CMesh::CreateVaosAndShit()
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	meshVbo = vbo;
	meshVao = vao;

}

IShader* CMesh::CreateShader(SShaderParams* pShaderParams)
{
	if (pShaderParams->id == 0)
	{
		pShaderParams->id = rand() * 1000;
	}

	if (pShaderParams->name == "")
	{
		gSys->Log("[SHADERSYS] You didn't name your shader!");
		return nullptr;
	}

	if (pShaderParams->f_file == "")
	{
		gSys->Log("[SHADERSYS] No frag shader path specified!");
		return nullptr;
	}

	if (pShaderParams->v_file == "")
	{
		gSys->Log("[SHADERSYS] No vert shader path specified!");
		return nullptr;
	}

	CShader* pNShader = new CShader(pShaderParams);

	if (CMeshSystem* pMeshSys = gSys->pMeshSystem)
	{
		// Control the shaderz.
		pMeshSys->AddToShaderContainer(pNShader);
	} else return nullptr;

	return pNShader;
}

void CMesh::BuildTM(Vec3 pos, glm::vec3 axis, float rot)
{
	m_tm = glm::translate(glm::mat4(), pos) * glm::rotate(glm::mat4(), rot, axis);
}
