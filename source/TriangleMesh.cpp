#include "TriangleMesh.h"

#include <iostream>


TriangleMesh::TriangleMesh(const FPoint3& position, const std::shared_ptr<Material>& material,
                           const std::vector<FPoint3>& verticesBuffer, const std::vector<unsigned int>& indexBuffer, CullingMode cullingMode)
	: Geometry(position, material)
	, m_VerticesBuffer(verticesBuffer)
	, m_IndexBuffer(indexBuffer)
	, m_CullingMode(cullingMode)
{
	CreateTriangles();
}

bool TriangleMesh::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	bool hasHit = false;
	HitRecord lastHit{};

	for (const Triangle& triangle : m_Triangles)
	{
		if (triangle.Hit(ray, hitRecord))
		{
			// if we hit something closer than the origin, we can be sure no other hit will be closer
			const float squaredPositionToRayOrigin = SqrDistance(m_Position, ray.origin);
			if (squaredPositionToRayOrigin > (hitRecord.t * hitRecord.t))
			{
				return true;
			}

			// In case we hit something behind the origin, there might be closer hit
			hasHit = true;
			lastHit = hitRecord;
		}
	}

	hitRecord = lastHit;
	return hasHit;
}

void TriangleMesh::CreateTriangles()
{
	if (m_IndexBuffer.size() % 3 != 0)
	{
		std::cout << "Mesh indexBuffer size issue: size is not fully divisible by 3" << std::endl;
	}

	for (int i{0}; i < m_IndexBuffer.size(); i += 3)
	{
		m_Triangles.emplace_back(m_Position, m_pMaterial,
			m_VerticesBuffer[i],
			m_VerticesBuffer[i + 1],
			m_VerticesBuffer[i + 2],
			m_CullingMode);
	}
}
