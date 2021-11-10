#include "Sphere.h"

#include <utility>

Sphere::Sphere(const FPoint3& position, std::shared_ptr<Material> pMaterial, float radius)
	: Geometry(position, std::move(pMaterial))
	, m_Radius{radius}
	, m_InvertedRadius(1.f / radius) // so we don't keep recalculating this
{
}

bool Sphere::Hit(const Ray& ray, HitRecord& hitRecord) const
{
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