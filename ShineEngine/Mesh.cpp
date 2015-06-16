// Implements IMesh.

#include "shine.h"
#include "Mesh.h"
#include "tiny_obj_loader.h"
#include "Shader.h"
#include "MeshSystem.h"

#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include <glm\gtc\matrix_transform.hpp>
#include "MaterialSystem.h"


CMesh::CMesh(SMeshParams* pMesh)
{
	m_meshId = pMesh->id;
	m_meshName = pMesh->name;
	m_file = pMesh->fileName;
	CreateVaosAndShit();
	m_worldPos = pMesh->pos;
	m_worldRotAxis = pMesh->rotaxis;
	m_worldRotScalar = pMesh->rotAmmount;
	if(IMaterial* pMaterial = gSys->pMaterialSystem->LoadMaterial("m.mtl"))
		m_pMaterial = pMaterial;
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRotScalar);
}

CMesh::~CMesh() {}

void CMesh::SetPos(Vec3 pos)
{
	m_worldPos = pos;
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRotScalar);
}

void CMesh::SetRotation(glm::vec3 axis, float rot)
{
	m_worldRotAxis = axis;
	m_worldRotScalar = rot;
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRotScalar);
}

void CMesh::CreateVaosAndShit()
{

	std::string inputfile = m_file;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

	if (!err.empty()) 
	{
		gSys->Log("[MESHSYS] Cannot find the .obj file specified.");
		exit(1);
	}	

	m_verticies = shapes[m_slot].mesh.positions;
	m_indiciesVector = shapes[m_slot].mesh.indices;
	m_normals = shapes[m_slot].mesh.normals;
	m_texcoords = shapes[m_slot].mesh.texcoords;

	GLuint vbo = 0;
	GLuint indicies = 0;
	GLuint normals = 0;
	GLuint tex_coords = 0; // TODO: Fix shit variable naming

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (!m_verticies.empty())
	{
		glEnableVertexAttribArray(0);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, (GLubyte *) NULL);
	}

	if (!m_indiciesVector.empty())
	{
		glGenBuffers(1, &indicies);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indiciesVector.size() * sizeof(uint), &m_indiciesVector[0], GL_STATIC_DRAW);
	}

	if (!m_normals.empty())
	{
		//glEnableVertexAttribArray(1);
		glGenBuffers(1, &normals);
		glBindBuffer(GL_ARRAY_BUFFER, normals);
		glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(float), &m_normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GL_FALSE, (GLubyte *) NULL);
	}

	if (!m_texcoords.empty())
	{
		glEnableVertexAttribArray(2);
		glGenBuffers(1, &tex_coords);
		glBindBuffer(GL_ARRAY_BUFFER, tex_coords);
		glBufferData(GL_ARRAY_BUFFER, m_texcoords.size() * sizeof(float), &m_texcoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	glBindVertexArray(NULL);


	meshVbo = vbo;
	meshVao = vao;
	meshInidcies = indicies;
	meshNormals = normals;
	meshTexcoords = tex_coords;

}

void CMesh::BuildTM(Vec3 pos, glm::vec3 axis, float rot)
{
	m_tm = glm::translate(glm::mat4(), pos) * glm::rotate(glm::mat4(), rot, axis);
}
