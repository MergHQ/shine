#include "Texture.h"
#include <freeimg\imageloader.hpp>

CTexture::CTexture(STextureParams* params)
{
	m_texFile = params->m_file;
	m_texName = params->m_name;
	m_texId = params->m_id;
	m_texType = params->m_Type;

	Load();
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &m_texBufferId);
}

void CTexture::Load()
{
	string file = ASSET_ROOT_DIR + m_texFile;

	Image_t img;
	if (file.c_str() != nullptr && file != "")
	{
		img.load(file.c_str());
	}

	GLuint textureID = 0;
	glGenTextures(1, &textureID);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	m_texBufferId = textureID;
}

void CTexture::ActivateTexture(GLuint activate, GLuint uniformLoc, bool cubemap)
{
	if (!cubemap)
	{
		glActiveTexture(activate);
		glBindTexture(GL_TEXTURE_2D, m_texBufferId);
		int offset = activate - 0x84C0;
		glUniform1i(uniformLoc, offset);
	}
}

CCubeMapTexture::CCubeMapTexture(STextureParams* params)
{
	for (int i = 0; i < 6; i++)
	{
		m_textureFiles[i] = params->cubemapTextures[i];
	}

	m_textureName = params->m_name;
	m_textureId = params->m_id;


	m_types[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	m_types[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	m_types[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	m_types[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	m_types[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	m_types[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

	Load();
}

CCubeMapTexture::~CCubeMapTexture()
{
}

void CCubeMapTexture::Load()
{
	// Load textures
	unsigned char header[54];
	unsigned char * data;
	int dataPos;
	int imageSize;
	int width;
	int height;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (int i = 0; i < 6; i++)
	{
		string file = ASSET_ROOT_DIR + m_textureFiles[i];
		Image_t img;
		if (file.c_str() != nullptr && file != "")
		{
			img.load(file.c_str());
		}

		glTexImage2D(m_types[i], 0, GL_SRGB8_ALPHA8, img.width, img.height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, img.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}


	m_texBufferId = textureID;
}

void CCubeMapTexture::ActivateTexture(GLuint activate, GLuint uniformLoc, bool cubemap)
{
	if (cubemap)
	{
		glActiveTexture(activate);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texBufferId);
		int offset = activate - 0x84C0;
		glUniform1i(uniformLoc, offset);
	}
}

CFboTexture::CFboTexture(GLint cbt, GLint dataType, GLint colorAttachment, int width, int height)
{
	Load(cbt, dataType, colorAttachment, width, height);
}

CFboTexture::~CFboTexture()
{
	glDeleteTextures(1, &m_texBufferId);
}

void CFboTexture::Load(GLint cbt, GLint dataType, GLint colorAttachment, int width, int height)
{
	int shit;

	if (cbt == GL_RGB32F)
		shit = GL_RGB;
	else if (cbt == GL_DEPTH32F_STENCIL8)
		shit = GL_DEPTH_STENCIL;
	else shit = cbt;

	glGenTextures(1, &m_texBufferId);
	glBindTexture(GL_TEXTURE_2D, m_texBufferId);
	glTexImage2D(GL_TEXTURE_2D, 0, cbt, width, height, 0, shit, dataType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, colorAttachment, m_texBufferId, 0);
}

void CFboTexture::ActivateTexture(GLuint activate, GLuint uniformLoc, bool cubemap)
{
	if (!cubemap)
	{
		glActiveTexture(activate);
		glBindTexture(GL_TEXTURE_2D, m_texBufferId);
		int offset = activate - 0x84C0;
		glUniform1i(uniformLoc, offset);
	}
}
