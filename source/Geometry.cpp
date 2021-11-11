#include "Geometry.h"

Geometry::Geometry(const FPoint3& position, std::shared_ptr<Material> pMaterial) //const Elite::RGBColor& color
	: m_Position{position}
	, m_pMaterial(pMaterial)
{
}

void Geometry::SetPosition(const FPoint3& position)
{
	m_Position = position;
}
const FPoint3& Geometry::GetPosition() const
{
	return m_Position;
}

void Geometry::SetMaterial(std::shared_ptr<Material> pMaterial)
{
	m_pMaterial = std::move(pMaterial);
}

std::shared_ptr<Material> Geometry::GetMaterial() const
{
	return m_pMaterial;
}
