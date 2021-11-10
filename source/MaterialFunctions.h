#pragma once
#include "ERGBColor.h"
#include "EMath.h"


namespace BRDF
{
	// Helper
	inline FVector3 CreateHalfVector(const FVector3& lightDirection, const FVector3& viewDirection)
	{
		return GetNormalized(viewDirection + lightDirection);
	}

	// Material Functions
	inline RGBColor Lambert(const float diffuseReflectance, const Elite::RGBColor& diffuseColor)
	{
		return (diffuseColor * diffuseReflectance) / static_cast<float>(E_PI);
	}

	inline RGBColor Phong(float specularReflectance, int phongExponent, const FVector3& incomingLightDirection, 
	                      const FVector3& viewDirection, const FVector3& hitNormal)
	{
		const FVector3 reflect = Reflect(incomingLightDirection, hitNormal);
		const float angleBetween = Dot(reflect, viewDirection);
		const float phongSpecularReflectance = specularReflectance * powf(angleBetween,static_cast<float>(phongExponent));
		return {phongSpecularReflectance, phongSpecularReflectance, phongSpecularReflectance};
	}

	inline RGBColor FresnelFunction_Schlick(const FVector3& lightDirection, const FVector3& viewDirection, const RGBColor& baseReflectivity)
	{
		const FVector3 halfVector = CreateHalfVector(lightDirection, viewDirection);
		return baseReflectivity + (RGBColor(1, 1, 1) - baseReflectivity) * powf(1 - Dot(halfVector, viewDirection), 5.0f);
	}

	inline float NormalDistribution_GGX(const FVector3& lightDirection, const FVector3& viewDirection, const FVector3& hitNormal, float roughness)
	{
		const float alpha = roughness * roughness;
		const float alphaSquared = alpha * alpha;
		const FVector3 halfVector = CreateHalfVector(lightDirection, viewDirection);
		const float dotNormalHalfVector = Dot(hitNormal, halfVector);

		return static_cast<float>(alphaSquared) / static_cast<float>(E_PI * pow(pow(dotNormalHalfVector, 2) * (alphaSquared - 1) + 1, 2));
	}

	inline float GeometryFunction_SchlickGGX(const FVector3& viewDirection, const FVector3& hitNormal, float roughness)
	{
		const float roughnessSquared = roughness * roughness;
		const float kDirect = static_cast<float>(pow(roughnessSquared + 1, 2)) / 8;
		const float dotNormalView = Dot(hitNormal, viewDirection);

		return dotNormalView / (dotNormalView * (1 - kDirect) + kDirect);
	}

	inline  float GeometryFunction_Smith(const FVector3& normal, const FVector3& viewDirection, const FVector3& lightDirection, float roughness)
	{
		return GeometryFunction_SchlickGGX(normal, viewDirection, roughness) * 
			GeometryFunction_SchlickGGX(normal, lightDirection, roughness);
	}
}
