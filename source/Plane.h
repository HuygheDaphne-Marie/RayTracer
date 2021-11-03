#pragma once
#include "Object.h"
class Plane : public Object
{
public:
	Plane(const Elite::FPoint3& position, const Elite::FVector3& normal, const Elite::RGBColor& color = {1,1,0});
	virtual ~Plane() = default;

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	void SetNormal(const Elite::FVector3& normal);
	const Elite::FVector3& GetNormal();

protected:
	Elite::FVector3 m_Normal;
};

