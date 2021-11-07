#include "LambertPhongMaterial.h"
#include "MaterialFunctions.h"

LambertPhongMaterial::LambertPhongMaterial(const RGBColor& color, float diffuseReflectance, float specularReflectance, int phongExponent)
	: LambertMaterial(color, diffuseReflectance)
	, m_SpecularReflectance(specularReflectance)
	, m_PhongExponent(phongExponent)
{
}

RGBColor LambertPhongMaterial::Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + 
		BRDF::Phong(m_SpecularReflectance, m_PhongExponent, incomingLightDirection, viewDirection, hitRecord.normal);
}

void LambertPhongMaterial::SetPhongExponent(int exponent)
{
	m_PhongExponent = exponent;
}

void LambertPhongMaterial::SetSpecularReflectance(float reflectance)
{
	// Ensure [0, 1] range
	if (reflectance < 0.f)
	{
		m_SpecularReflectance = 0.f;
		return;
	}
	if (reflectance > 1.0f)
	{
		m_SpecularReflectance = 1.f;
		return;
	}

	m_SpecularReflectance = reflectance;
}
