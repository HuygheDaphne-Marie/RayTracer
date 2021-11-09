#include "Sphere.h"

Sphere::Sphere(const FPoint3& position, Material* pMaterial, float radius)
	: Geometry(position, pMaterial)
	, m_Radius{radius}
	, m_InvertedRadius(1.f / radius) // so we don't keep recalculating this
{
}

bool Sphere::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	//const Elite::FVector3 toSphereCentre{ m_Position - ray.origin };
	//const float rejectedDistanceSquared{ Elite::SqrMagnitude(Elite::Reject(toSphereCentre, ray.direction)) };
	//if (rejectedDistanceSquared > pow(m_Radius, 2))
	//{
	//	return false;
	//}
	//
	//const float rejectedDistance{ sqrtf(rejectedDistanceSquared) };
	//const float tca{ Elite::Dot(toSphereCentre, ray.direction) };
	//const float thc{ m_Radius - rejectedDistance };
	//
	//const float t0 = tca - thc;
	//const float t1 = tca + thc;
	//
	//const float closest = std::min(t0, t1);
	//if (closest > ray.tMin && ray.tMax > closest && closest < hitRecord.t)
	//{
	//	hitRecord.t = closest;
	//	hitRecord.pMaterial = m_pMaterial;
	//	hitRecord.hitPoint = ray.origin + ray.direction * hitRecord.t;
	//	hitRecord.normal = (hitRecord.hitPoint - m_Position) * m_InvertedRadius;
	//	return true;
	//}
	//
	//return false;

	// Week 1 sphere intersect
	const FVector3 toSphereCenter{ ray.origin - m_Position };
	const float a{ 1.0f }; // Normalized vector always gives 1 as square magnitude (1² = 1 * 1 = 1)
	const float b{ Dot((2 * ray.direction), toSphereCenter) };
	const float c{ Dot(toSphereCenter, toSphereCenter) - (m_Radius * m_Radius) };

	const float discriminant = (b * b) - 4 * a * c; // b² - 4ac
	if (discriminant > 0)
	{
		const float t = (-b - sqrtf(discriminant)) / 2 * a;
		// HitRecord.t starts at tMax, so by checking if we're smaller than it we check if we're closer and if we're in the interval
		if (t > ray.tMin && t < hitRecord.t)
		{
			hitRecord.t = t;
			hitRecord.pMaterial = m_pMaterial;
			hitRecord.hitPoint = ray.origin + ray.direction * hitRecord.t;
			hitRecord.normal = (hitRecord.hitPoint - m_Position) * m_InvertedRadius;
			return true;
		}
	}
	return false;
}

void Sphere::SetRadius(float radius)
{
	m_Radius = radius;
	m_InvertedRadius = 1.f / radius;
}
float Sphere::GetRadius() const
{
	return m_Radius;
}