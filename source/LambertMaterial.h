#pragma once
#include "Material.h"

class LambertMaterial : public Material
{
public:
	LambertMaterial(const RGBColor& color, float diffuseReflectance);
	~LambertMaterial() override = default;

	RGBColor Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const override;


	void SetColor(const RGBColor& color);
	void SetDiffuseReflectance(float reflectance);

protected:
	RGBColor m_DiffuseColor{};
	float m_DiffuseReflectance{};
};

