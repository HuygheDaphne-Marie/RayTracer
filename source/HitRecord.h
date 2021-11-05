#pragma once
#include "ERGBColor.h"
#include "EMath.h"

struct HitRecord
{
	float t{};
	Elite::RGBColor color{};
	Elite::FPoint3 hitPoint{};
	Elite::FVector3 normal{};
};