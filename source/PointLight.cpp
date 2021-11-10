#include "PointLight.h"

PointLight::PointLight(const FPoint3& position, const RGBColor& color, float intensity)
	: Light(color, intensity)
	, m_Position(position) 
{
}

float PointLight::CalculateIrradianceAtPoint(const FPoint3& point) const
{
	return m_LightIntensity / SqrDistance(m_Position, point);
}

RGBColor PointLight::CalculateIrradiance(const FPoint3& point) const
{
	return m_Color * CalculateIrradianceAtPoint(point);
}

FVector3 PointLight::GetDirectionNormalised(const FPoint3& to) const
{
	return GetNormalized(GetDirection(to));
}

FVector3 PointLight::GetDirection(const FPoint3& to) const
{
	return FVector3{ m_Position - to };
}
