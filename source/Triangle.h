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
		const FPoint3& vertex1, const FPoint3& vertex2, CullingMode cullingMode = CullingMode::None, const FVector3& forward = { 0,0,1 });

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	void RecalculateTransformation();
	void UpdateRotation(float timeSinceStart);

private:
	FVector3 m_Forward;
	FMatrix4 m_Transform{};

	const CullingMode m_CullingMode;

	const FVector3 m_Normal;
	FVector3 m_TransformedNormal;

	const FPoint3 m_Vertex0;
	const FPoint3 m_Vertex1;
	const FPoint3 m_Vertex2;

	FPoint3 m_TransformedVertex0;
	FPoint3 m_TransformedVertex1;
	FPoint3 m_TransformedVertex2;
};

