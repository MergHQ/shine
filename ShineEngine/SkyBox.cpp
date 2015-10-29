#include "SkyBox.h"
#include "Shader.h"
#include "shine.h"
#include "tiny_obj_loader.h"

CSkyBox::CSkyBox()
{

	SShaderParams s;
	s.s_file = "shaders/skybox.ss";
	s.name = "dsdasadsa";
	pShader = new CShader(&s);

	STextureParams t;
	t.cubemapTextures[0] = "irradiance/posx.jpg";
	t.cubemapTextures[1] = "irradiance/negx.jpg";
	t.cubemapTextures[2] = "irradiance/posy.jpg";
	t.cubemapTextures[3] = "irradiance/negy.jpg";
	t.cubemapTextures[4] = "irradiance/posz.jpg";
	t.cubemapTextures[5] = "irradiance/negz.jpg";
	t.m_name = "dskldkjmfkdls";
	tex = new CCubeMapTexture(&t);

	std::string inputfile = ASSET_ROOT_DIR + "objects/cube.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

	if (!err.empty())
	{
		gSys->Log("[MESHSYS] Cannot find the .obj file specified.");
		return;
	}

	m_verticies = shapes[0].mesh.positions;
	m_indiciesVector = shapes[0].mesh.indices;
	m_normals = shapes[0].mesh.normals;
	m_texcoords = shapes[0].mesh.texcoords;

	GLuint vbo = 0;
	GLuint indicies = 0;
	GLuint normals = 0;
	GLuint tex_coords = 0;

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (!m_verticies.empty())
	{
		glEnableVertexAttribArray(0);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, (GLubyte *)NULL);
	}

	if (!m_indiciesVector.empty())
	{
		glGenBuffers(1, &indicies);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indiciesVector.size() * sizeof(uint), &m_indiciesVector[0], GL_STATIC_DRAW);
	}

	if (!m_normals.empty())
	{
		glEnableVertexAttribArray(1);
		glGenBuffers(1, &normals);
		glBindBuffer(GL_ARRAY_BUFFER, normals);
		glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(float), &m_normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GL_FALSE, (GLubyte *)NULL);
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

	m_vao = vao;
}

CSkyBox::~CSkyBox()
{
	delete pShader;
	delete tex;
	glDeleteBuffers(1, &m_vao);
}

void CSkyBox::Draw()
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(pShader->GetShaderProgramme());

	glBindVertexArray(m_vao);

	glUniformMatrix4fv(glGetUniformLocation(pShader->GetShaderProgramme(), "MVP"), 1, GL_FALSE, glm::value_ptr(gSys->GetCamera()->GetVPMatrix() * (glm::translate(Mat44(), cam->GetWorldPos())) * glm::rotate(Mat44(),0.0f, Vec3(1,0,0)) * glm::scale(Mat44(), Vec3(900,900,900))));

	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex->GetTextureBufferId());
	glUniform1i(glGetUniformLocation(pShader->GetShaderProgramme(), "cubemap"), 20);

	glDrawElements(GL_TRIANGLES, m_indiciesVector.size() * sizeof(uint), GL_UNSIGNED_INT, 0);

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);

	glUseProgram(NULL);
}
