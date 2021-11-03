#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Ray.h"
#include "HitRecord.h"

class Object
{
public:
	Object(const Elite::FPoint3& position, const Elite::RGBColor& color);
	virtual ~Object() = default;

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const = 0;

	void SetPosition(const Elite::FPoint3& position);
	const Elite::FPoint3& GetPosition();

	void SetColor(const Elite::RGBColor& color);
	const Elite::RGBColor& GetColor();

protected:
	Elite::FPoint3 m_Position;
	Elite::RGBColor m_Color;
};

