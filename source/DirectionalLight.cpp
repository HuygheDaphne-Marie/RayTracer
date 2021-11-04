#include "DirectionalLight.h"

RGBColor DirectionalLight::CalculateIrradiance(const FPoint3& point) const
{
	return m_Color * m_LightIntensity;
}

FVector3 DirectionalLight::GetDirection(const FPoint3& to) const
{
	return m_Direction;
}
