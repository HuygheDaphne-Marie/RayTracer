#pragma once
#include "ERGBColor.h"
#include "EMath.h"

namespace BRDF
{
	inline Elite::RGBColor Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor)
	{
		return (diffuseColor * diffuseReflectance) / static_cast<float>(E_PI);
	}
}
