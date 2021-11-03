#pragma once
#include "Object.h"
#include <vector>

#include "Ray.h"
#include "HitRecord.h"

class Scene final
{
public:
	Scene();
	~Scene();

	void AddObjectToScene(Object* object);

	const std::vector<Object*>& GetObjects() const;

	const bool Hit(const Ray& ray, HitRecord& hitRecord) const;

private:
	std::vector<Object*> m_pObjects;
};

