#pragma once
#include "Geometry.h"
#include "Light.h"
#include <vector>

#include "Ray.h"
#include "HitRecord.h"
#include "PerspectiveCamera.h"

// single class which will hold
// - geometry
// - materials
// - lights
// - camera

class SceneGraph final
{
public:
	SceneGraph() = default;
	// Todo: fix this
	//SceneGraph(const SceneGraph& other);
	//SceneGraph(SceneGraph&& other);
	//SceneGraph& operator=(const SceneGraph& other);
	//SceneGraph& operator=(SceneGraph&& other);
	~SceneGraph();

	bool Hit(const Ray& ray, HitRecord& hitRecord) const;

	void AddGeometryToScene(Geometry* pObject);
	const std::vector<Geometry*>& GetGeometries() const;

	void AddLightToScene(Light* pLight);
	const std::vector<Light*>& GetLights() const;

	void InitialiseCamera(const float width, const float height, const float fieldOfViewAngleDegrees, const FPoint3& position);
	PerspectiveCamera* GetCamera() const;

private:
	std::vector<Geometry*> m_pGeometries{};
	std::vector<Light*> m_pLights{};

	PerspectiveCamera* m_pPerspectiveCamera{ nullptr };
};

