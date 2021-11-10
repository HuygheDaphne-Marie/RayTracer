#pragma once
#include "EMath.h"
#include "ERGBColor.h"

using namespace Elite;

class Light
{
public:
	Light(const RGBColor& color, float intensity)
		: m_Color(color)
		, m_LightIntensity(intensity)
	{
	}
	virtual ~Light() = default;

	virtual RGBColor CalculateIrradiance(const FPoint3& point) const = 0;
	virtual FVector3 GetDirectionNormalised(const FPoint3& to) const = 0;
	virtual FVector3 GetDirection(const FPoint3& to) const = 0;

protected:
	RGBColor m_Color{};
	float m_LightIntensity{};
};

