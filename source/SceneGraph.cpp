#include "SceneGraph.h"


SceneGraph::~SceneGraph()
{
	for (Geometry* pGeometry : m_pGeometries)
	{
		delete pGeometry;
		pGeometry = nullptr;
	}

	for (Light* pLight : m_pLights)
	{
		delete pLight;
		pLight = nullptr;
	}

	delete m_pPerspectiveCamera;
	m_pPerspectiveCamera = nullptr;
}

void SceneGraph::AddGeometryToScene(Geometry* pObject)
{
	if (pObject == nullptr)
	{
		return;
	}
	m_pGeometries.push_back(pObject);
}
void SceneGraph::AddLightToScene(Light* pLight)
{
	if (pLight == nullptr)
	{
		return;
	}
	m_pLights.push_back(pLight);
}

const std::vector<Geometry*>& SceneGraph::GetGeometries() const
{
	return m_pGeometries;
}
const std::vector<Light*>& SceneGraph::GetLights() const
{
	return m_pLights;
}

bool SceneGraph::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	bool hasHit{ false };
	for (Geometry* pObject : GetGeometries())
	{
		if (pObject->Hit(ray, hitRecord))
			hasHit = true;
	}
	return hasHit;
}

void SceneGraph::InitialiseCamera(const float width, const float height, const float fieldOfViewAngleDegrees, const FPoint3& position)
{
	delete m_pPerspectiveCamera; // Deleting a nullptr has no effect
	m_pPerspectiveCamera = new PerspectiveCamera(width, height, fieldOfViewAngleDegrees, position);
}
PerspectiveCamera* SceneGraph::GetCamera() const
{
	return m_pPerspectiveCamera;
}
