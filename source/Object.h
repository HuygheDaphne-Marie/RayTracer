#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Material.h"

class Object
{
public:
	Object(const Elite::FPoint3& position, Material* pMaterial);
	// Todo: Rule of 5
	virtual ~Object();

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const = 0;

	void SetPosition(const Elite::FPoint3& position);
	const Elite::FPoint3& GetPosition();

	void SetMaterial(Material* pMaterial);
	Material* GetMaterial() const;

protected:
	Elite::FPoint3 m_Position;
	Material* m_pMaterial; // could do shared ptr?
};

