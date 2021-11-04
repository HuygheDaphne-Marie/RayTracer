#pragma once
#include "Object.h"
#include "Light.h"
#include <vector>

#include "Ray.h"
#include "HitRecord.h"

class Scene final
{
public:
	Scene();
	~Scene();

	void AddObjectToScene(Object* pObject);
	void AddLightToScene(Light* pLight);

	const std::vector<Object*>& GetObjects() const;
	const std::vector<Light*>& GetLights() const;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const;

private:
	std::vector<Object*> m_pObjects;
	std::vector<Light*> m_pLights;
};

