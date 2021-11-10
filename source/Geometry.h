#pragma once
#include "EMath.h"
#include "Ray.h"
#include "HitRecord.h"
#include <memory>

using namespace Elite;

class Geometry
{
public:
	Geometry(const FPoint3& position, std::shared_ptr<Material> pMaterial);
	virtual ~Geometry() = default;

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const = 0;

	void SetPosition(const FPoint3& position);
	const FPoint3& GetPosition();

	void SetMaterial(std::shared_ptr<Material> pMaterial);
	std::shared_ptr<Material> GetMaterial() const;

protected:
	FPoint3 m_Position;
	std::shared_ptr<Material> m_pMaterial;
};

