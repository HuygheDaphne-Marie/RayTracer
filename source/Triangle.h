#pragma once
#include "Geometry.h"

enum class CullingMode
{
	None, // Default
	Front,
	Back
};

class Triangle final : public Geometry
{
public:
	Triangle(const FPoint3& position, const std::shared_ptr<Material>& material, const FPoint3& vertex0, 
		const FPoint3& vertex1, const FPoint3& vertex2, CullingMode cullingMode = CullingMode::None);

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;


private:
	const FVector3 m_Normal;
	const CullingMode m_CullingMode;

	const FPoint3& m_Vertex0;
	const FPoint3& m_Vertex1;
	const FPoint3& m_Vertex2;
};

