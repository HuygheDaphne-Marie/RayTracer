#include "Plane.h"

Plane::Plane(const Elite::FPoint3& position, const Elite::FVector3& normal, Material* pMaterial)
	: Geometry(position, pMaterial)
	, m_Normal{ Elite::GetNormalized(normal) }
{
}

bool Plane::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	const float t = Elite::Dot((m_Position - ray.origin), m_Normal) / Elite::Dot(ray.direction, m_Normal);
	if (t > hitRecord.t || t < ray.tMin)
	{
		return false;
	}

	//const Elite::FPoint3 intersectPoint{ray.origin + ray.direction * t};
	//const Elite::FVector3 toIntersectPoint{ intersectPoint - m_Position };
	//const float dotResult{ Elite::Dot(toIntersectPoint, m_Normal) };
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

void Plane::SetNormal(const Elite::FVector3& normal)
{
	m_Normal = Elite::GetNormalized(normal);
}
const Elite::FVector3& Plane::GetNormal()
{
	return m_Normal;
}