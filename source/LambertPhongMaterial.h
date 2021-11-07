#pragma once
#include "LambertMaterial.h"
class LambertPhongMaterial final : public LambertMaterial
{
public:
	LambertPhongMaterial(const RGBColor& color, float diffuseReflectance, float specularReflectance, int phongExponent);
	~LambertPhongMaterial() override = default;
	
	RGBColor Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const override;

	void SetPhongExponent(int exponent);
	void SetSpecularReflectance(float reflectance);

private:
	float m_SpecularReflectance{};
	int m_PhongExponent{};
};

