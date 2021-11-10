#pragma once
#include "Geometry.h"


class Sphere final : public Geometry
{
public:
	Sphere(const FPoint3& position, std::shared_ptr<Material> pMaterial, float radius);
	virtual ~Sphere() = default;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	void SetRadius(float radius);
	float GetRadius() const;

private:
	float m_Radius;
	float m_InvertedRadius;
};

