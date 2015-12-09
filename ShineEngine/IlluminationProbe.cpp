#include "IlluminationProbe.h"
#include <Windows.h>
#include "Renderer.h"
#include <glm\gtc\matrix_transform.hpp>
#include <math.h>

#define Y0(n)   (0.282095f)                             /* L_00 */
#define Y1(n)   (0.488603f*n.y)                         /* L_1-1 */
#define Y2(n)   (0.488603f*n.z)                         /* L_10 */
#define Y3(n)   (0.488603f*n.x)                         /* L_11 */
#define Y4(n)   (1.092548f*n.x*n.y)                     /* L_2-2 */
#define Y5(n)   (1.092548f*n.y*n.z)                     /* L_2-1 */
#define Y6(n)   (0.315392f*(3.0f*n.z*n.z - 1.0f))       /* L_20 */
#define Y7(n)   (1.092548f*n.x*n.z)                     /* L_21 */
#define Y8(n)   (0.546274f*(n.x*n.x - n.y*n.y))         /* L_22 */

#define RED     0u
#define GREEN   1u
#define BLUE    2u

CIlluminationProbe::CIlluminationProbe(std::vector<string>& filename, Vec3 position)
{
	float shCoeff[3][9];
	memset(shCoeff[RED], 0, 9u*sizeof(float));
	memset(shCoeff[GREEN], 0, 9u*sizeof(float));
	memset(shCoeff[BLUE], 0, 9u*sizeof(float));

	CEnvTexture* envMaps;
	if (!filename.empty())
	{
		envMaps = new CEnvTexture(filename);

		// Make radiance map.
		STextureParams t;
		for (int index = 0; index < 6; index++)
			t.cubemapTextures[index] = filename[index];
		t.m_name = "dskldkjmfkdls";
		m_pCmTex = new CCubeMapTexture(&t);
	}
	else return;

	float texelSize = 1 / envMaps->size[0];
	float texRes = envMaps->size[0];

	// Color from data.
	const float dColor = 1.0f / float((sizeof(unsigned char) << 8) - 1);
	 
	float sumWeight = 0.0f;

	for (int tex = 0; tex < 6; tex++)
	{
		float u, v;
		unsigned char *pixels = envMaps->data[tex];

		for (int i = 0; i < texRes; i++)
		{
			v = 2.0f * ((i + 0.5f) * texelSize) - 1;

			for (int j = 0; j < texRes; j++)
			{
				u = 2.0f * ((j + 0.5f) * texelSize) - 1.0f;

				Vec3 dir; float solidAngle;
				GetTexelAttrib(tex, u, v, texelSize, &dir, &solidAngle);
				sumWeight += solidAngle;

				float theta;

				// Integration of formula 10

				// Add coefficients to the matrix.
				theta = (pixels[RED] * dColor) * solidAngle;
				shCoeff[RED][0] += theta * Y0(dir);
				shCoeff[RED][1] += theta * Y1(dir);
				shCoeff[RED][2] += theta * Y2(dir);
				shCoeff[RED][3] += theta * Y3(dir);
				shCoeff[RED][4] += theta * Y4(dir);
				shCoeff[RED][5] += theta * Y5(dir);
				shCoeff[RED][6] += theta * Y6(dir);
				shCoeff[RED][7] += theta * Y7(dir);
				shCoeff[RED][8] += theta * Y8(dir);

				theta = (pixels[GREEN] * dColor) * solidAngle;
				shCoeff[GREEN][0] += theta * Y0(dir);
				shCoeff[GREEN][1] += theta * Y1(dir);
				shCoeff[GREEN][2] += theta * Y2(dir);
				shCoeff[GREEN][3] += theta * Y3(dir);
				shCoeff[GREEN][4] += theta * Y4(dir);
				shCoeff[GREEN][5] += theta * Y5(dir);
				shCoeff[GREEN][6] += theta * Y6(dir);
				shCoeff[GREEN][7] += theta * Y7(dir);
				shCoeff[GREEN][8] += theta * Y8(dir);

				theta = (pixels[BLUE] * dColor) * solidAngle;
				shCoeff[BLUE][0] += theta * Y0(dir);
				shCoeff[BLUE][1] += theta * Y1(dir);
				shCoeff[BLUE][2] += theta * Y2(dir);
				shCoeff[BLUE][3] += theta * Y3(dir);
				shCoeff[BLUE][4] += theta * Y4(dir);
				shCoeff[BLUE][5] += theta * Y5(dir);
				shCoeff[BLUE][6] += theta * Y6(dir);
				shCoeff[BLUE][7] += theta * Y7(dir);
				shCoeff[BLUE][8] += theta * Y8(dir);

				pixels += 3;
			}
		}
	}

	const float domega = 2.0f * PI / sumWeight;
	for (int i = 0; i < 9; ++i)
	{
		shCoeff[RED][i] *= domega;
		shCoeff[GREEN][i] *= domega;
		shCoeff[BLUE][i] *= domega;
	}

	const float c1 = 0.429043f;
	const float c2 = 0.511664f;
	const float c3 = 0.743125f;
	const float c4 = 0.886227f;
	const float c5 = 0.247708f;

	Mat44 M[3];

	for (int c = 0; c<3; ++c)
	{
		M[c][0][0] = c1 * shCoeff[c][8];
		M[c][0][1] = c1 * shCoeff[c][4];
		M[c][0][2] = c1 * shCoeff[c][7];
		M[c][0][3] = c2 * shCoeff[c][3];

		M[c][1][0] = c1 * shCoeff[c][4];
		M[c][1][1] = -c1 * shCoeff[c][8];
		M[c][1][2] = c1 * shCoeff[c][5];
		M[c][1][3] = c2 * shCoeff[c][1];

		M[c][2][0] = c1 * shCoeff[c][7];
		M[c][2][1] = c1 * shCoeff[c][5];
		M[c][2][2] = c3 * shCoeff[c][6];
		M[c][2][3] = c2 * shCoeff[c][2];

		M[c][3][0] = c2 * shCoeff[c][3];
		M[c][3][1] = c2 * shCoeff[c][1];
		M[c][3][2] = c2 * shCoeff[c][2];
		M[c][3][3] = c4 * shCoeff[c][0] - c5 * shCoeff[c][6];
	}

	m_values = new IrradianceValues;

	m_values->m_red = M[0];
	m_values->m_green = M[1];
	m_values->m_blue = M[2];

	delete envMaps;
}

CIlluminationProbe::~CIlluminationProbe()
{
	delete m_values;
	delete m_pCmTex;
}

void CIlluminationProbe::GenerateImageData(Vec3 position, BYTE & imageData)
{
	
	
	float theta[6]= { 0, PI / 2, PI, PI * 1.5,0,0 };
	float omega[6] = { 0, 0, 0, PI / 2, PI * 1.5 };

	// Render scene for each face.
	for (int i = 0; i < 6; i++)
	{
		Vec3 cameraDir = Vec3(cos(omega[i]) * sin(theta[i]), sin(omega[i]), cos(omega[i]) * cos(theta[i]));

		// The resolution is 128px * 128px.
		Mat44 projMatrix = glm::perspective(45.0f, 128.0f / 128.0f, 0.1f, 4000.0f);
		Mat44 viewMatrix = glm::lookAt(position, position + cameraDir, Vec3(0, 1, 0));
	}
}

void CIlluminationProbe::GetTexelAttrib(const int texId, const float u, const float v, const float texelSize,
	Vec3 *direction, float *solidAngle)
{
	enum
	{
		POSITIVE_X, NEGATIVE_X,
		POSITIVE_Y, NEGATIVE_Y,
		POSITIVE_Z, NEGATIVE_Z
	};


	/// Compute the texel direction  
	switch (texId)
	{
	case POSITIVE_X:
		*direction = glm::vec3(+1.0f, -v, -u);
		break;

	case NEGATIVE_X:
		*direction = glm::vec3(-1.0f, -v, +u);
		break;

	case POSITIVE_Y:
		*direction = glm::vec3(+u, +1.0f, +v);
		break;

	case NEGATIVE_Y:
		*direction = glm::vec3(+u, -1.0f, -v);
		break;

	case POSITIVE_Z:
		*direction = glm::vec3(+u, -v, +1.0f);
		break;

	case NEGATIVE_Z:
		*direction = glm::vec3(-u, -v, -1.0f);
		break;
	}

	*direction = glm::normalize(*direction);

	float x0 = u - texelSize;
	float y0 = v - texelSize;
	float x1 = u + texelSize;
	float y1 = v + texelSize;

#define AREA(x, y)  atan2f(x * y, sqrtf(x * x + y * y + 1.0))
	*solidAngle = (AREA(x0, y0) + AREA(x1, y1)) - (AREA(x0, y1) + AREA(x1, y0));
#undef AREA  
}
CEnvTexture::CEnvTexture(std::vector<string>& f)
{
	unsigned char header[54];
	unsigned char * imgData;
	int dataPos;
	int imageSize;
	int width;
	int height;

	for(string f1 : f)
	{ 
		string file = ASSET_ROOT_DIR + f1;

		Image_t* image = new Image_t;
		assert(true == image->load(file.c_str()));
		data.push_back(image->data);
		size.push_back(image->width);
		shits.push_back(image);
	}
}

CEnvTexture::~CEnvTexture()
{
	for (Image_t* a : shits)
	{
		delete a;
	}
}

#undef Y0
#undef Y1
#undef Y2
#undef Y3
#undef Y4
#undef Y5
#undef Y6
#undef Y7
#undef Y8

#undef RED
#undef GREEN
#undef BLUE  

CLookUpTexture::CLookUpTexture()
{
	// Some defs
	unsigned const WIDTH = 32;
	unsigned const HEIGHT = 64;
	unsigned const sampleNum = 128;

	float lutData[WIDTH][HEIGHT][4];

	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		float const ndotv = (y + 0.5f) / HEIGHT;

		for (unsigned x = 0; x < WIDTH; ++x)
		{
			float gloss = (x + 0.5f) / WIDTH;
			float roughness = powf(1.0f - gloss, 2.0f);

			float vx = sqrtf(1.0f - ndotv * ndotv);
			float vy = 0.0f;
			float vz = ndotv;

			float scale = 0.0f;
			float bias = 0.0f;


			for (unsigned i = 0; i < sampleNum; ++i)
			{
				float e1 = (float)i / sampleNum;
				float e2 = (float)((double)ReverseBits(i) / (double)0x100000000LL);

				float phi = 2.0f * PI * e1;
				float cosPhi = cosf(phi);
				float sinPhi = sinf(phi);
				float cosTheta = sqrtf((1.0f - e2) / (1.0f + (roughness * roughness - 1.0f) * e2));
				float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);

				float hx = sinTheta * cosf(phi);
				float hy = sinTheta * sinf(phi);
				float hz = cosTheta;

				float vdh = vx * hx + vy * hy + vz * hz;
				float lx = 2.0f * vdh * hx - vx;
				float ly = 2.0f * vdh * hy - vy;
				float lz = 2.0f * vdh * hz - vz;

				float ndotl = glm::max(lz, 0.0f);
				float ndoth = glm::max(hz, 0.0f);
				float vdoth = glm::max(vdh, 0.0f);

				if (ndotl > 0.0f)
				{
					// Apply the geometric attenuation for the values
					float gsmith = GSmith(roughness, ndotv, ndotl);
					float ndotlVisPDF = ndotl * gsmith * (4.0f * vdoth / ndoth);
					float fc = powf(1.0f - vdoth, 5.0f);

					scale += ndotlVisPDF * (1.0f - fc);
					bias += ndotlVisPDF * fc;
				}
			}
			scale /= sampleNum;
			bias /= sampleNum;
			lutData[x][y][0] = scale;
			lutData[x][y][1] = bias;
			lutData[x][y][2] = 0.0f;
			lutData[x][y][3] = 0.0f;
		}
	}

	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, lutData);
	glGenerateMipmap(GL_TEXTURE_2D);
	gSys->Log(std::to_string(glGetError()));
}

CLookUpTexture::~CLookUpTexture()
{
}

float CLookUpTexture::GSmith(float roughness, float ndotv, float ndotl)
{
	float m2 = roughness * roughness;
	float visV = ndotl * sqrt(ndotv * (ndotv - ndotv * m2) + m2);
	float visL = ndotv * sqrt(ndotl * (ndotl - ndotl * m2) + m2);
	return 0.5f / (visV + visL);
}

uint32_t CLookUpTexture::ReverseBits(uint32_t v)
{
	v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
	v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
	v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
	v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
	v = (v >> 16) | (v << 16);
	return v;
}
