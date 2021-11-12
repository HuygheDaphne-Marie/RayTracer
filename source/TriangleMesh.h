#pragma once
#include "Geometry.h"
#include <vector>
#include <string>

#include "EMath.h"
#include "LambertMaterial.h"
#include "Triangle.h"
#include "Face.h"

class TriangleMesh final : public Geometry
{
public:
	TriangleMesh(const FPoint3& position, const std::shared_ptr<Material>& material, 
	             FVector3 forward, const std::string& filename, 
	             CullingMode cullingMode = CullingMode::None);

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;
	bool TriangleHit(const Ray& ray, HitRecord& hitRecord, const Face& face) const;

	void RecalculateTransformation();
	void Rotate(float elapsedSec);

private:
	FVector3 m_Forward;
	FMatrix4 m_Transform{};

	std::vector<FPoint3> m_VerticesBuffer;
	std::vector<FPoint3> m_TransformedVerticesBuffer;
	std::vector<Face> m_Faces;


	std::vector<unsigned int> m_IndexBuffer;
	std::vector<FVector3> m_NormalBuffer;

	const CullingMode m_CullingMode;
	std::vector<Triangle> m_Triangles; // Todo: remove

	void AssembleMesh(const std::vector<FPoint3>& vertices, std::vector<Face>& faces);
};

