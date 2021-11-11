#pragma once
#include "Geometry.h"
#include <vector>

#include "EMath.h"
#include "Triangle.h"

class TriangleMesh final : public Geometry
{
public:
	TriangleMesh(const FPoint3& position, const std::shared_ptr<Material>& material,
		const std::vector<FPoint3>& verticesBuffer, const std::vector<unsigned int>& indexBuffer, CullingMode cullingMode);

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;


private:
	std::vector<FPoint3> m_VerticesBuffer;
	std::vector<unsigned int> m_IndexBuffer;

	const CullingMode m_CullingMode;
	std::vector<Triangle> m_Triangles;

	void CreateTriangles();
};

