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
}

void Scene::AddObjectToScene(Object* object)
{
	if (object == nullptr)
	{
		return;
	}
	m_pObjects.push_back(object);
}

const std::vector<Object*>& Scene::GetObjects() const
{
	return m_pObjects;
}

const bool Scene::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	bool hasHit{ false };
	for (Object* pObject : GetObjects())
	{
		if (pObject->Hit(ray, hitRecord))
			hasHit = true;
	}
	return hasHit;
}