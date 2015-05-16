// Implements IMesh.

#include "shine.h"
#include "Mesh.h"
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include <glm\gtc\matrix_transform.hpp>


CMesh::CMesh(SMeshParams* pMesh)
{
	m_meshId = pMesh->id;
	m_meshName = pMesh->name;
	m_file = pMesh->fileName;
	m_textureFile = pMesh->textureFile;
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

	std::string inputfile = m_file;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

	if (!err.empty()) 
	{
		gSys->Log("[MESHSYS] Cannot find the object file specified.");
		exit(1);
	}	

	m_verticies = shapes[m_slot].mesh.positions;
	m_indiciesVector = shapes[m_slot].mesh.indices;
	m_normals = shapes[m_slot].mesh.normals;

	GLuint vbo = 0;
	GLuint indicies = 0;
	GLuint normals = 0;

	if (!m_verticies.empty())
	{

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);
	}

	if (!m_indiciesVector.empty())
	{
		
		glGenBuffers(1, &indicies);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indiciesVector.size() * sizeof(float), &m_indiciesVector[0], GL_STATIC_DRAW);
	}

	if (!m_normals.empty())
	{
		
		glGenBuffers(1, &normals);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normals);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_normals.size() * sizeof(float), &m_normals[0], GL_STATIC_DRAW);

	}
		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);

	// Load textures
	unsigned char header[54]; // 54 byte header
	unsigned char * data;
	if (m_textureFile != nullptr && m_textureFile != "")
	{
		FILE* pFile = fopen(m_textureFile, "rb");

		if (pFile)
		{
			if (fread(header, 1, 54, pFile) != 54)
				gSys->Log("Not a BMP file");

			if (header[0]!= 'B' || header[1]!= 'M')
			{
				gSys->Log("Not a BMP file (Wrong header)");
			}

			dataPos = *(int*)&(header[0x0A]);
			imageSize = *(int*)&(header[0x22]);
			width = *(int*)&(header[0x12]);
			height = *(int*)&(header[0x16]);

			if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
			if (dataPos == 0)      dataPos = 54;

			data = new unsigned char[imageSize];

			fread(data, 1, imageSize, pFile);

			fclose(pFile);
		}
		else gSys->Log("Cannot open file \n");
	}

	

	meshVbo = vbo;
	meshVao = vao;
	meshInidcies = indicies;
	meshNormals = normals;

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
