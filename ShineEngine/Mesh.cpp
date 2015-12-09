// Implements IMesh.
#include "Mesh.h"
#include "shine.h"
#include "tiny_obj_loader.h"
#include "Shader.h"
#include "MeshSystem.h"
#include "MaterialSystem.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <ostream>
#include <fstream>

CMesh::CMesh(SMeshParams* pMesh)
{
	m_meshId = pMesh->id;
	m_meshName = pMesh->name;
	m_file = pMesh->fileName;
	m_materialFile = pMesh->m_materialFile;
	CreateBufferObjects();
	m_worldPos = pMesh->pos;
	m_worldRotAxis = pMesh->rotaxis;
	m_worldRotScalar = pMesh->rotAmmount;
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRotScalar);
}

CMesh::~CMesh() 
{
	for (uint i = 0; i < m_shapeContainer.size(); i++)
	{
		glDeleteBuffers(1, &m_shapeContainer[i]->meshInidcies); 
		glDeleteBuffers(1, &m_shapeContainer[i]->meshNormals);
		glDeleteBuffers(1, &m_shapeContainer[i]->meshTexcoords);
		glDeleteBuffers(1, &m_shapeContainer[i]->meshVao);
		glDeleteBuffers(1, &m_shapeContainer[i]->meshVbo);
	}
}

void CMesh::SetPos(Vec3 pos)
{
	m_worldPos = pos;
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRotScalar);
}

void CMesh::SetRotation(Vec3 axis, float rot)
{
	m_worldRotAxis = axis;
	m_worldRotScalar = rot;
	BuildTM(m_worldPos, m_worldRotAxis, m_worldRotScalar);
}

void CMesh::CreateBufferObjects()
{
	//std::filebuf fileBuffer;
	//fileBuffer.open(m_file,std::ios::in);

	//std::istream stream(&fileBuffer);

	//std::vector<tinyobj::shape_t> shapes = ReadCompiledObj(&stream);

	//fileBuffer.close();

	bool shouldLoad = true;

	for (uint i = 0; i < gSys->pMeshSystem->GetMeshContainer().size(); i++)
	{
		if (gSys->pMeshSystem->GetMeshContainer()[i]->GetFileName() == m_file && !gSys->pMeshSystem->GetMeshContainer()[i]->shapes.empty())
		{
			shapes = gSys->pMeshSystem->GetMeshContainer()[i]->shapes;
			tangents = gSys->pMeshSystem->GetMeshContainer()[i]->tangents;
			shouldLoad = false;
		}
	}

	if (shouldLoad)
	{
		gSys->Log("Loading .obj " + m_file);

		std::string inputfile = ASSET_ROOT_DIR + m_file;
		std::vector<tinyobj::material_t> materials;

		std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

		if (!err.empty())
		{
			gSys->Log("[MESHSYS] Cannot find the .obj file specified.");
			return;
		}
	}
	
	
	for (uint i = 0; i < shapes.size(); i++)
	{

		auto currentShape = shapes[i];

		std::vector<Vec3> vertices;
		std::vector<Vec2> texCoords;

		for (uint j = 0; j < currentShape.mesh.positions.size(); j += 3)
		{
			vertices.push_back(Vec3(currentShape.mesh.positions[j], currentShape.mesh.positions[j + 1], currentShape.mesh.positions[j + 2]));
		}

		if (!currentShape.mesh.texcoords.empty())
			for (uint k = 0; k < currentShape.mesh.texcoords.size(); k += 2)
			{
				texCoords.push_back(Vec2(currentShape.mesh.texcoords[k], currentShape.mesh.texcoords[k + 1]));
			}

		GLuint vbo = 0;
		GLuint indicies = 0;
		GLuint normals = 0;
		GLuint tex_coords = 0; // TODO: Fix shit variable naming

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		if (!currentShape.mesh.positions.empty())
		{
			glEnableVertexAttribArray(0);
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, (GLubyte *)NULL);

		}

		if (!currentShape.mesh.indices.empty())
		{
			glGenBuffers(1, &indicies);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentShape.mesh.indices.size() * sizeof(uint), &currentShape.mesh.indices[0], GL_STATIC_DRAW);
		}

		if (!currentShape.mesh.normals.empty())
		{
			glEnableVertexAttribArray(1);
			glGenBuffers(1, &normals);
			glBindBuffer(GL_ARRAY_BUFFER, normals);
			glBufferData(GL_ARRAY_BUFFER, currentShape.mesh.normals.size() * sizeof(float), &currentShape.mesh.normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GL_FALSE, (GLubyte *)NULL);
		}

		if (!currentShape.mesh.texcoords.empty())
		{
			glEnableVertexAttribArray(2);
			glGenBuffers(1, &tex_coords);
			glBindBuffer(GL_ARRAY_BUFFER, tex_coords);
			glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(Vec2), &texCoords[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		}

		if (!currentShape.mesh.texcoords.empty() && shouldLoad)
		{
			auto payload = ComputeTangent(vertices, texCoords);
			tangents = payload[0];
		}

		if (!tangents.empty())
		{
			// Gen tangents
			glEnableVertexAttribArray(3);
			glGenBuffers(1, &tex_coords);
			glBindBuffer(GL_ARRAY_BUFFER, tex_coords);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vec3), &tangents[0][0], GL_STATIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		}
		
		
		glBindVertexArray(NULL);

		// Fill the shape struct
		auto shapeInstance = new Shape;
		shapeInstance->indices = currentShape.mesh.indices;
		shapeInstance->meshInidcies = indicies;
		shapeInstance->meshNormals = normals;
		shapeInstance->meshTexcoords = tex_coords;
		shapeInstance->meshVao = vao;
		shapeInstance->meshVbo = vbo;
		shapeInstance->name = currentShape.name;
		shapeInstance->pMaterial = gSys->pMaterialSystem->LoadMaterial(m_materialFile.c_str(), currentShape.name);

		m_shapeContainer.push_back(shapeInstance);

	}
}

void CMesh::BuildTM(Vec3 pos, Vec3 axis, float rot)
{
	m_tm = glm::translate(glm::mat4(), pos) * glm::rotate(glm::mat4(), rot, axis) * glm::scale(Mat44(),Vec3(1,1,1));
}

std::vector<std::vector<Vec3>> CMesh::ComputeTangent(std::vector<Vec3>& pos, std::vector<Vec2>& uv)
{
	std::vector<Vec3> tan;
	std::vector<Vec3> bitan;

	//for (int i = 0; i < pos.size(); i += 3)
	//{
	//	if (i + 1 <= pos.size() && i + 2 <= pos.size() && i + 1 <= uv.size() && i + 2 <= uv.size())
	//	{
	//		Vec3& v0 = pos[i];
	//		Vec3& v1 = pos[i + 1];
	//		Vec3& v2 = pos[i + 2];

	//		Vec2& uv0 = uv[i];
	//		Vec2& uv1 = uv[i + 1];
	//		Vec2& uv2 = uv[i + 2];

	//		// Vertex delta
	//		Vec3 dpos1 = v1 - v0;
	//		Vec3 dpos2 = v2 - v0;

	//		// UV delta
	//		Vec2 duv1 = uv1 - uv0;
	//		Vec2 duv2 = uv2 - uv0;

	//		// Formula http://ogldev.atspace.co.uk/www/tutorial26/tangent_space_calc6.jpg

	//		float r = 1.0f / ((duv1.x * duv2.y) - (duv2.x - duv1.y));
	//		Vec3 tangent = (dpos1 * duv2.y - dpos2 * duv1.y)*r;
	//		Vec3 bitangent = (dpos2 * duv1.x - dpos1 * duv2.x)*r;

	//		tangent = glm::normalize(tangent);
	//		bitangent = glm::normalize(bitangent);

	//		tan.push_back(tangent);
	//		tan.push_back(tangent);
	//		tan.push_back(tangent);

	//		bitan.push_back(bitangent);
	//		bitan.push_back(bitangent);
	//		bitan.push_back(bitangent);
	//	}
	//}

	std::vector<std::vector<Vec3>> last;
	last.push_back(tan);
	last.push_back(bitan);
	return last;
}


std::vector<tinyobj::shape_t> CMesh::ReadCompiledObj(std::istream* stream)
{
	assert(sizeof(float) == sizeof(uint32_t));
	const auto sz = sizeof(uint32_t);

	std::vector<tinyobj::shape_t> shapes;

	uint32_t nMeshes = 0;
	uint32_t nMatProperties = 0;
	stream->read((char*)&nMeshes, sz);
	shapes.resize(nMeshes);
	for (size_t i = 0; i < nMeshes; ++i) {
		uint32_t nVertices = 0, nNormals = 0, nTexcoords = 0, nIndices = 0;
		stream->read((char*)&nVertices, sz);
		stream->read((char*)&nNormals, sz);
		stream->read((char*)&nTexcoords, sz);
		stream->read((char*)&nIndices, sz);

		shapes[i].mesh.positions.resize(nVertices);
		shapes[i].mesh.normals.resize(nNormals);
		shapes[i].mesh.texcoords.resize(nTexcoords);
		shapes[i].mesh.indices.resize(nIndices);

		if (!shapes[i].mesh.positions.empty())
			stream->read((char*)&shapes[i].mesh.positions[0], nVertices  * sz);
		if (!shapes[i].mesh.normals.empty())
			stream->read((char*)&shapes[i].mesh.normals[0], nNormals   * sz);
		if (!shapes[i].mesh.texcoords.empty())
			stream->read((char*)&shapes[i].mesh.texcoords[0], nTexcoords * sz);
		if (!shapes[i].mesh.indices.empty())
			stream->read((char*)&shapes[i].mesh.indices[0], nIndices   * sz);
	}
	return shapes;
}