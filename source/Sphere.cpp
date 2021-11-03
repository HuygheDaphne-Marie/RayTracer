#include "Sphere.h"

Sphere::Sphere(const Elite::FPoint3& position, const Elite::RGBColor& color, float radius)
	: Object(position, color)
	, m_Radius{radius}
{
}

bool Sphere::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	const Elite::FVector3 toSphereCentre{ m_Position - ray.origin };
	const float rejectedDistanceSquared{ Elite::SqrMagnitude(Elite::Reject(toSphereCentre, ray.direction)) };
	if (rejectedDistanceSquared > pow(m_Radius, 2))
	{
		return false;
	}

	const float rejectedDistance{ sqrtf(rejectedDistanceSquared) };
	const float tca{ Elite::Dot(toSphereCentre, ray.direction) };
	const float thc{ m_Radius - rejectedDistance };

	const float t0 = tca - thc;
	const float t1 = tca + thc;

	const float closest = std::min(t0, t1);
	if (closest > ray.tMin && ray.tMax > closest && closest < hitRecord.t)
	{
		hitRecord.t = closest;
		hitRecord.color = m_Color;
		return true;
	}

	return false;
}

void Sphere::SetRadius(float radius)
{
	m_Radius = radius;
}
float Sphere::GetRadius() const
{
	return m_Radius;
}