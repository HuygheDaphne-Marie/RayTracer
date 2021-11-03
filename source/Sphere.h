#pragma once
#include "Object.h"


class Sphere final : public Object
{
public:
	Sphere(const Elite::FPoint3& position, const Elite::RGBColor& color, float radius);
	virtual ~Sphere() = default;

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

	void SetRadius(float radius);
	float GetRadius() const;

protected:
	float m_Radius;
};

