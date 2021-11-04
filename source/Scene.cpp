#include "Scene.h"

Scene::Scene()
	: m_pObjects{}
{
}
Scene::~Scene()
{
	for (Object* pObject : m_pObjects)
	{
		delete pObject;
		pObject = nullptr;
	}

	for (Light* pLight : m_pLights)
	{
		delete pLight;
		pLight = nullptr;
	}
}

void Scene::AddObjectToScene(Object* pObject)
{
	if (pObject == nullptr)
	{
		return;
	}
	m_pObjects.push_back(pObject);
}
void Scene::AddLightToScene(Light* pLight)
{
	if (pLight == nullptr)
	{
		return;
	}
	m_pLights.push_back(pLight);
}

const std::vector<Object*>& Scene::GetObjects() const
{
	return m_pObjects;
}
const std::vector<Light*>& Scene::GetLights() const
{
	return m_pLights;
}

bool Scene::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	bool hasHit{ false };
	for (Object* pObject : GetObjects())
	{
		if (pObject->Hit(ray, hitRecord))
			hasHit = true;
	}
	return hasHit;
}
