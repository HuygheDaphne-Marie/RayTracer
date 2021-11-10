#pragma once
#include "Material.h"
class PBRMaterial final : public Material
{
public:
	PBRMaterial(const RGBColor& albedoColor, bool isMetal, float roughness);

	RGBColor Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const override;

	void SetRoughness(float roughness);

private:
	RGBColor m_AlbedoColor;
	bool m_isMetal;
	float m_Roughness;

	RGBColor GetBaseReflectivity() const;
};

