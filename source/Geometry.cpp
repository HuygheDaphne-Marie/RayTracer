#include "Geometry.h"

Geometry::Geometry(const Elite::FPoint3& position, std::shared_ptr<Material> pMaterial) //const Elite::RGBColor& color
	: m_Position{position}
	, m_pMaterial(pMaterial)
{
}

void Geometry::SetPosition(const Elite::FPoint3& position)
{
	m_Position = position;
}
const Elite::FPoint3& Geometry::GetPosition()
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
