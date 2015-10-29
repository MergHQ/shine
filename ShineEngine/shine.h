#ifndef _SHINE_H_
#define _SHINE_H_
#pragma once

#define sl std;
#define DEV_MODE
#define ASSET_ROOT_DIR std::string("data/")
#define PI float(3.14159265358979323846264338327950288)

#include "GlobalSystem.h"
#include <glm\glm.hpp>

// A whole bunch of typedefs. 
typedef std::string string;
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;
typedef glm::quat Quat;
typedef glm::mat2 Mat2;
typedef glm::mat3x4 Mat34;
typedef glm::mat4x4 Mat44;
typedef unsigned int uint;

extern IGlobalSystem* gSys;

#endif