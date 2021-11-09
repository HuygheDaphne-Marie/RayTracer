#include "Geometry.h"

Geometry::Geometry(const Elite::FPoint3& position, Material* pMaterial) //const Elite::RGBColor& color
	: m_Position{position}
	, m_pMaterial(pMaterial)
{
}

Geometry::~Geometry()
{
	delete m_pMaterial;
	m_pMaterial = nullptr;
}

void Geometry::SetPosition(const Elite::FPoint3& position)
{
	m_Position = position;
}
const Elite::FPoint3& Geometry::GetPosition()
{
	return m_Position;
}

void Geometry::SetMaterial(Material* pMaterial)
{
	if(pMaterial == m_pMaterial)
		return;

	delete m_pMaterial;
	m_pMaterial = pMaterial;
}

Material* Geometry::GetMaterial() const
{
	return m_pMaterial;
}
