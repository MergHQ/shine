#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include "shine.h"

// EG. Sun
struct DirectionalLight
{
	Vec3 position;
	Vec3 color;
};

// For anything else. These lights sources should be calculated in post processing.
struct PointLight
{
	Vec3 position;
	Vec3 color;
};

#endif