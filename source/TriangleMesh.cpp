#include "TriangleMesh.h"
#include <iostream>
#include "OBJReader.h"

TriangleMesh::TriangleMesh(const FPoint3& position, const std::shared_ptr<Material>& material, FVector3 forward,
	const std::string& filename, CullingMode cullingMode)
	: Geometry(position, material)
	, m_Forward(GetNormalized(forward))
	, m_CullingMode(cullingMode)
{
	std::vector<FPoint3> objVertices;
	std::vector<Face> objFaces;

	OBJReader::GetInstance().ReadOBJ(filename, objVertices, objFaces);
	AssembleMesh(objVertices, objFaces);

	m_TransformedVerticesBuffer.clear();
	m_TransformedVerticesBuffer.resize(m_VerticesBuffer.size());
	RecalculateTransformation();
}

bool TriangleMesh::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	bool hasHit{ false };
	for (const Face& face : m_Faces)
	{
		if (TriangleHit(ray, hitRecord, face))
		{
			hasHit = true;
		}
	}
	return hasHit;
}

bool TriangleMesh::TriangleHit(const Ray& ray, HitRecord& hitRecord, const Face& face) const
{
	const FPoint3 vertex0{ m_TransformedVerticesBuffer[face.verticesIndex[0]] };
	const FPoint3 vertex1{ m_TransformedVerticesBuffer[face.verticesIndex[1]] };
	const FPoint3 vertex2{ m_TransformedVerticesBuffer[face.verticesIndex[2]] };
	const FVector3 normal = face.transformedNormal;

	const float dotNormalView = Dot(normal, ray.direction);

	switch (m_CullingMode)
	{
	case CullingMode::Back:
		if (dotNormalView > 0)
			return false;
		break;
	case CullingMode::Front:
		if (dotNormalView < 0)
			return false;
		break;
	}

	if (abs(dotNormalView) < FLT_EPSILON) // if dotNormalView == 0
		return false;

	FPoint3 center
	{
		(vertex0.x + vertex1.x + vertex2.x) / 3.f,
		(vertex0.y + vertex1.y + vertex2.y) / 3.f,
		(vertex0.z + vertex1.z + vertex2.z) / 3.f
	};

	const float t = Dot((center - ray.origin), normal) / Dot(ray.direction, normal);
	if (t < ray.tMin || t > ray.tMax || t > hitRecord.t)
		return false;

	FPoint3 intersectPoint = ray.origin + ray.direction * t;


	FVector3 intersectToVertex{ intersectPoint - vertex0 };
	FVector3 edge{ vertex1 - vertex0 };
	if (Dot(normal, Cross(edge, intersectToVertex)) < 0.f) // edgeA
		return false;

	intersectToVertex = intersectPoint - vertex1;
	edge = { vertex2 - vertex1 };
	if (Dot(normal, Cross(edge, intersectToVertex)) < 0.f) // edgeB
		return false;

	intersectToVertex = intersectPoint - vertex2;
	edge = { vertex0 - vertex2 };
	if (Dot(normal, Cross(edge, intersectToVertex)) < 0.f)
		return false;

	hitRecord.t = t;
	hitRecord.hitPoint = intersectPoint;
	hitRecord.pMaterial = m_pMaterial;
	hitRecord.normal = normal; // might need to worry about this with culling..
	if (dotNormalView > 0) // turn normal to camera, so triangles aren't black
	{
		hitRecord.normal *= -1;
	}

	return true;
}

void TriangleMesh::RecalculateTransformation()
{
	const FVector3 worldUp{ 0,1,0 };

	const FVector3 right{ Cross(worldUp, m_Forward) };
	const FVector3 up{ Cross(m_Forward, right) };

	m_Transform[0] = FVector4{ right, 0 };
	m_Transform[1] = FVector4{ up, 0 };
	m_Transform[2] = FVector4{ m_Forward, 0 };
	m_Transform[3] = FVector4{ m_Position.x, m_Position.y, m_Position.z, 1.f };

	for (int i{0}; i < m_VerticesBuffer.size(); i++)
	{
		FPoint4 vertex = { m_VerticesBuffer[i].x, m_VerticesBuffer[i].y, m_VerticesBuffer[i].z, 1 };
		vertex = m_Transform * vertex;
		m_TransformedVerticesBuffer[i].x = vertex.x;
		m_TransformedVerticesBuffer[i].y = vertex.y;
		m_TransformedVerticesBuffer[i].z = vertex.z;
	}

	for (Face& face : m_Faces)
	{
		face.transformedNormal = FVector3{ m_Transform * FVector4{face.normal, 0} };
	}
}

void TriangleMesh::Rotate(float elapsedSec)
{
	m_Forward = MakeRotationY(ToRadians(45.f * elapsedSec)) * m_Forward;
	RecalculateTransformation();
}

void TriangleMesh::AssembleMesh(const std::vector<FPoint3>& vertices, std::vector<Face>& faces)
{
	// Clear old mesh and make space
	m_VerticesBuffer.clear();
	m_VerticesBuffer.reserve(vertices.size());
	m_Faces.clear();
	m_Faces.reserve(faces.size());

	// Copy verticesIndex to mesh
	m_VerticesBuffer.insert(m_VerticesBuffer.end(), vertices.begin(), vertices.end());

	for (Face& face : faces)
	{
		face.normal = GetNormalized(
			Cross(
				m_VerticesBuffer[face.verticesIndex[1]] - m_VerticesBuffer[face.verticesIndex[0]],
				m_VerticesBuffer[face.verticesIndex[2]] - m_VerticesBuffer[face.verticesIndex[1]]
			));

		face.transformedNormal = face.normal;
		m_Faces.push_back(face);
	}
}

