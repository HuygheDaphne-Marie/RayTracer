#include "Triangle.h"

Triangle::Triangle(const FPoint3& position, const std::shared_ptr<Material>& material, const FPoint3& vertex0,
	const FPoint3& vertex1, const FPoint3& vertex2, CullingMode cullingMode)
	: Geometry(position, material)
	, m_Normal(GetNormalized(Cross(vertex1 - vertex0, vertex2 - vertex1)))
	, m_CullingMode(cullingMode)
	, m_Vertex0(vertex0)
	, m_Vertex1(vertex1)
	, m_Vertex2(vertex2)
{
}

bool Triangle::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	// Adjust for position, I dislike doing this here
	const FPoint3 vertex0{ m_Position + FVector3{m_Vertex0} };
	const FPoint3 vertex1{ m_Position + FVector3{m_Vertex1} };
	const FPoint3 vertex2{ m_Position + FVector3{m_Vertex2} };

	const float dotNormalView = Dot(m_Normal, ray.direction);

	switch (m_CullingMode)
	{
	case CullingMode::Back:
		if(dotNormalView > 0)
			return false;
		break;
	case CullingMode::Front:
		if (dotNormalView < 0)
			return false;
		break;
	}

	if (abs(dotNormalView) < FLT_EPSILON) // if dotNormalView == 0
		return false;

	FPoint3 center
	{
		(vertex0.x + vertex1.x + vertex2.x) / 3.f,
		(vertex0.y + vertex1.y + vertex2.y) / 3.f,
		(vertex0.z + vertex1.z + vertex2.z) / 3.f
	};

	const float t = Dot((center - ray.origin), m_Normal) / Dot(ray.direction, m_Normal);
	if (t < ray.tMin || t > ray.tMax || t > hitRecord.t)
		return false;

	FPoint3 intersectPoint = ray.origin + ray.direction * t;


	FVector3 intersectToVertex{ intersectPoint - vertex0 };
	FVector3 edge{ vertex1 - vertex0 };
	if (Dot(m_Normal, Cross(edge, intersectToVertex)) < 0.f) // edgeA
		return false;

	intersectToVertex = intersectPoint - vertex1;
	edge = { vertex2 - vertex1 };
	if (Dot(m_Normal, Cross(edge, intersectToVertex)) < 0.f) // edgeB
		return false;

	intersectToVertex = intersectPoint - vertex2;
	edge = { vertex0 - vertex2 };
	if (Dot(m_Normal, Cross(edge, intersectToVertex)) < 0.f)
		return false;

	hitRecord.t = t;
	hitRecord.hitPoint = intersectPoint;
	hitRecord.pMaterial = m_pMaterial;
	hitRecord.normal = m_Normal; // might need to worry about this with culling..

	return true;
}
