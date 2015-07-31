#include "Texture.h"

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
}

void CTexture::Load()
{
	// Load textures
	unsigned char header[54];
	unsigned char * data;
	int dataPos;
	int imageSize;
	int width;
	int height;
	string file = ASSET_ROOT_DIR + m_texFile;


	if (file.c_str() != nullptr && file.c_str() != "")
	{
		FILE* pFile = fopen(file.c_str(), "rb");

		if (pFile)
		{
			if (fread(header, 1, 54, pFile) != 54)
				gSys->Log("Not a BMP file");

			if (header[0] != 'B' || header[1] != 'M')
			{
				gSys->Log("Not a BMP file (Wrong header)");
			}

			dataPos = *(int*)&(header[0x0A]);
			imageSize = *(int*)&(header[0x22]);
			width = *(int*)&(header[0x12]);
			height = *(int*)&(header[0x16]);

			if (imageSize == 0)    imageSize = width*height * 3;
			if (dataPos == 0)      dataPos = 54;

			data = new unsigned char[imageSize];

			fread(data, 1, imageSize, pFile);

			fclose(pFile);
		}
		else 
			gSys->Log("Cannot open file");
	}


	GLuint textureID = 0;
	glGenTextures(1, &textureID);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);


	m_texBufferId = textureID;
	delete data;
}