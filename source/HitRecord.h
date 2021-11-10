#pragma once
#include "EMath.h"
#include <memory>

class Material;

struct HitRecord
{
	float t{ FLT_MAX };
	std::shared_ptr<Material> pMaterial{};
	Elite::FPoint3 hitPoint{};
	Elite::FVector3 normal{};
};