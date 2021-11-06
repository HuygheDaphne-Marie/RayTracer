#pragma once
#include "EMath.h"

class Material;

struct HitRecord
{
	float t{ FLT_MAX };
	Material* pMaterial{};
	Elite::FPoint3 hitPoint{};
	Elite::FVector3 normal{};
};