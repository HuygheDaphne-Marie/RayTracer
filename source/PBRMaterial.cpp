#include "PBRMaterial.h"
#include "MaterialFunctions.h"

PBRMaterial::PBRMaterial(const RGBColor& albedoColor, bool isMetal, float roughness)
	: m_AlbedoColor(albedoColor)
	, m_isMetal(isMetal)
	, m_Roughness(roughness)
{
}

RGBColor PBRMaterial::Shade(const HitRecord& hitRecord, const FVector3& incomingLightDirection, const FVector3& viewDirection) const
{
	const RGBColor baseReflectivity = GetBaseReflectivity();
	const RGBColor fresnel = BRDF::FresnelFunction_Schlick(incomingLightDirection, viewDirection, baseReflectivity);
	const float normalDistribution = BRDF::NormalDistribution_GGX(incomingLightDirection, viewDirection, hitRecord.normal, m_Roughness);
	const float geometry = BRDF::GeometryFunction_Smith(hitRecord.normal, viewDirection, incomingLightDirection, m_Roughness);
	const RGBColor specular = (fresnel * normalDistribution * geometry) / (4 * (Dot(viewDirection, hitRecord.normal) * Dot(incomingLightDirection, hitRecord.normal)));

	//if (m_isMetal)
	//	return specular;

	//kd
	const RGBColor diffuseReflectance = (m_isMetal) ? RGBColor{0,0,0} : RGBColor{ 1.f, 1.f, 1.f } - fresnel;
	const RGBColor lambert = BRDF::Lambert(diffuseReflectance, m_AlbedoColor);
	return lambert + specular;
}

RGBColor PBRMaterial::GetBaseReflectivity() const
{
	return (m_isMetal) ? m_AlbedoColor : RGBColor{ 0.04f, 0.04f , 0.04f };
}
