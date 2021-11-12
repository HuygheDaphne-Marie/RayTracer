#include "Triangle.h"

#include <cmath>

Triangle::Triangle(const FPoint3& position, const std::shared_ptr<Material>& material, const FPoint3& vertex0,
                   const FPoint3& vertex1, const FPoint3& vertex2, CullingMode cullingMode, const FVector3& forward)
	: Geometry(position, material)
	, m_Normal(GetNormalized(Cross(vertex1 - vertex0, vertex2 - vertex1)))
	, m_CullingMode(cullingMode)
	, m_Vertex0(vertex0)
	, m_Vertex1(vertex1)
	, m_Vertex2(vertex2)
	, m_Forward(forward)
	, m_TransformedVertex0()
	, m_TransformedVertex1()
	, m_TransformedVertex2()
	, m_TransformedNormal()
{
	RecalculateTransformation();
}

bool Triangle::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	// Adjust for position, I dislike doing this here
	const FPoint3 vertex0{ m_TransformedVertex0 };
	const FPoint3 vertex1{ m_TransformedVertex1 };
	const FPoint3 vertex2{ m_TransformedVertex2 };
	const FVector3 normal{ m_TransformedNormal };

	const float dotNormalView = Dot(normal, ray.direction);

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

	const float t = Dot((center - ray.origin), normal) / Dot(ray.direction, normal);
	if (t < ray.tMin || t > ray.tMax || t > hitRecord.t)
		return false;

	FPoint3 intersectPoint = ray.origin + ray.direction * t;


	FVector3 intersectToVertex{ intersectPoint - vertex0 };
	FVector3 edge{ vertex1 - vertex0 };
	if (Dot(normal, Cross(edge, intersectToVertex)) < 0.f) // edgeA
		return false;

	intersectToVertex = intersectPoint - vertex1;
	edge = { vertex2 - vertex1 };
	if (Dot(normal, Cross(edge, intersectToVertex)) < 0.f) // edgeB
		return false;

	intersectToVertex = intersectPoint - vertex2;
	edge = { vertex0 - vertex2 };
	if (Dot(normal, Cross(edge, intersectToVertex)) < 0.f)
		return false;

	hitRecord.t = t;
	hitRecord.hitPoint = intersectPoint;
	hitRecord.pMaterial = m_pMaterial;
	hitRecord.normal = normal; // might need to worry about this with culling..
	if (dotNormalView > 0) // turn normal to camera, so triangles aren't black
	{
		hitRecord.normal *= -1;
	}

	return true;
}

void Triangle::RecalculateTransformation()
{
	const FVector3 worldUp{ 0,1,0 };

	const FVector3 right{ Cross(worldUp, m_Forward) };
	const FVector3 up{ Cross(m_Forward, right) };

	m_Transform[0] = FVector4{ right, 0 };
	m_Transform[1] = FVector4{ up, 0 };
	m_Transform[2] = FVector4{ m_Forward, 0 };
	m_Transform[3] = FVector4{ m_Position.x, m_Position.y, m_Position.z, 1.f };

	FPoint4 vertex{ m_Vertex0.x, m_Vertex0.y, m_Vertex0.z, 1 };
	vertex = m_Transform * vertex;

	m_TransformedVertex0.x = vertex.x;
	m_TransformedVertex0.y = vertex.y;
	m_TransformedVertex0.z = vertex.z;

	vertex.xyz = m_Vertex1;
	vertex = m_Transform * vertex;
	m_TransformedVertex1.x = vertex.x;
	m_TransformedVertex1.y = vertex.y;
	m_TransformedVertex1.z = vertex.z;

	vertex.xyz = m_Vertex2;
	vertex = m_Transform * vertex;
	m_TransformedVertex2.x = vertex.x;
	m_TransformedVertex2.y = vertex.y;
	m_TransformedVertex2.z = vertex.z;

	m_TransformedNormal = FVector3{ m_Transform * FVector4{m_Normal, 0} };
}

void Triangle::UpdateRotation(float timeSinceStart)
{
	m_Forward = MakeRotationY(std::cos(timeSinceStart)) * m_Forward;
	RecalculateTransformation();
}
