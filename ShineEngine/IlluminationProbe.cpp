#include "IlluminationProbe.h"
#include <Windows.h>

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

CIlluminationProbe::CIlluminationProbe(std::vector<string>& filename, Mat44& out1, Mat44& out2, Mat44& out3)
{
	float shCoeff[3][9];
	memset(shCoeff[RED], 0, 9u*sizeof(float));
	memset(shCoeff[GREEN], 0, 9u*sizeof(float));
	memset(shCoeff[BLUE], 0, 9u*sizeof(float));

	auto envMaps = new CEnvTexture(filename);

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
	out1 = M[0];
	out2 = M[1];
	out3 = M[2];

	delete envMaps;
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
