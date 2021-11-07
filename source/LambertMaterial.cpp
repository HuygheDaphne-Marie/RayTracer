#include "LambertMaterial.h"

#include "MaterialFunctions.h"

LambertMaterial::LambertMaterial(const RGBColor& color, float diffuseReflectance)
	: m_DiffuseColor(color)
{
	SetDiffuseReflectance(diffuseReflectance);
}

RGBColor LambertMaterial::Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
}

void LambertMaterial::SetColor(const RGBColor& color)
{
	m_DiffuseColor = color;
}

void LambertMaterial::SetDiffuseReflectance(float reflectance)
{
	// Ensure [0, 1] range
	if (reflectance < 0.f)
	{
		m_DiffuseReflectance = 0.f;
		return;
	}
	if (reflectance > 1.0f)
	{
		m_DiffuseReflectance = 1.f;
		return;
	}

	m_DiffuseReflectance = reflectance;
}
