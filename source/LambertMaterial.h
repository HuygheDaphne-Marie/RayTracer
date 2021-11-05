#pragma once
#include "Material.h"

class LambertMaterial final : public Material
{
public:
	LambertMaterial(const RGBColor& color, float reflectance);
	~LambertMaterial() override = default;

	RGBColor Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const override;


	void SetColor(const RGBColor& color);
	void SetReflectance(float reflectance);

private:
	RGBColor m_DiffuseColor{};
	float m_DiffuseReflectance{};
};

