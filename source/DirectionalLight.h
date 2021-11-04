#pragma once
#include "Light.h"
class DirectionalLight final : public Light
{
public:
	DirectionalLight(const FVector3& direction, const RGBColor& color, float intensity)
		: Light(color, intensity)
		, m_Direction(direction)
	{
	}

	RGBColor LightAtPoint(const FPoint3& point) const override;
	FVector3 GetDirection(const FPoint3& to) const override;

private:
	FVector3 m_Direction{};
};

