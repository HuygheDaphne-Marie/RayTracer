#pragma once
#include "ERGBColor.h"
#include "EMath.h"
#include "HitRecord.h"

using namespace Elite;

class Material
{
public:
	virtual ~Material() = default;

	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const = 0;
};

