#pragma once
#include "Geometry.h"

class Plane final : public Geometry
{
public:
	Plane(const FPoint3& position, const FVector3& normal, std::shared_ptr<Material> pMaterial);
	~Plane() override = default;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	void SetNormal(const FVector3& normal);
	const FVector3& GetNormal() const;

protected:
	FVector3 m_Normal;
};

