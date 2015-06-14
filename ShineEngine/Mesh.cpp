// Implements IMesh.

#include "shine.h"
#include "Mesh.h"
#include "tiny_obj_loader.h"
#include "Shader.h"
#include "MeshSystem.h"

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
	m_worldRotScalar = pMesh->rotAmmount;
	m_pIShader = CreateShader(pMesh->pShader);
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

	// Load textures
	unsigned char header[54]; // 54 byte header for BMP files.
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
		else gSys->Log("Cannot open file");
	}


	GLuint textureID = 0;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glEnable(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	meshVbo = vbo;
	meshVao = vao;
	meshInidcies = indicies;
	meshNormals = normals;
	meshTexcoords = tex_coords;
	meshTextureId = textureID;

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

	if (pShaderParams->s_file == "")
	{
		gSys->Log("[SHADERSYS] No frag shader path specified!");
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
