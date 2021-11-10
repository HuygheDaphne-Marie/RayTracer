#include "Plane.h"

#include <utility>

Plane::Plane(const FPoint3& position, const FVector3& normal, std::shared_ptr<Material> pMaterial)
	: Geometry(position, std::move(pMaterial))
	, m_Normal{ GetNormalized(normal) }
{
}

bool Plane::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	const float t = Dot((m_Position - ray.origin), m_Normal) / Dot(ray.direction, m_Normal);
	if (t > hitRecord.t || t < ray.tMin)
	{
		return false;
	}

	//const FPoint3 intersectPoint{ray.origin + ray.direction * t};
	//const FVector3 toIntersectPoint{ intersectPoint - m_Position };
	//const float dotResult{ Dot(toIntersectPoint, m_Normal) };
	//if (-0.0001f < dotResult || dotResult < 0.0001f)
	//{
		hitRecord.t = t;
		hitRecord.pMaterial = m_pMaterial;
		hitRecord.hitPoint = ray.origin + ray.direction * hitRecord.t;
		hitRecord.normal = m_Normal;
		return true;
	//}
	//return false;
}

void Plane::SetNormal(const FVector3& normal)
{
	m_Normal = GetNormalized(normal);
}
const FVector3& Plane::GetNormal()
{
	return m_Normal;
}