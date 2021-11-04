#pragma once
#include "Light.h"

using namespace Elite;

class PointLight final : public Light
{
public:
	PointLight(const FPoint3& position, const RGBColor& color, float intensity);
	~PointLight() override = default;

	RGBColor LightAtPoint(const FPoint3& point) const override;
	FVector3 GetDirection(const FPoint3& to) const override;


private:
	FPoint3 m_Position{};
	
	float CalculateIrradianceAtPoint(const FPoint3& point) const;
};
