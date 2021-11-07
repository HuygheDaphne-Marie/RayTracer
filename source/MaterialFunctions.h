#pragma once
#include "ERGBColor.h"
#include "EMath.h"

namespace BRDF
{
	inline RGBColor Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor)
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
}
