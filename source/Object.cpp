#include "Object.h"

Object::Object(const Elite::FPoint3& position, Material* pMaterial) //const Elite::RGBColor& color
	: m_Position{position}
	, m_pMaterial(pMaterial)
{
}

Object::~Object()
{
	delete m_pMaterial;
	m_pMaterial = nullptr;
}

void Object::SetPosition(const Elite::FPoint3& position)
{
	m_Position = position;
}
const Elite::FPoint3& Object::GetPosition()
{
	return m_Position;
}

void Object::SetMaterial(Material* pMaterial)
{
	if(pMaterial == m_pMaterial)
		return;

	delete m_pMaterial;
	m_pMaterial = pMaterial;
}

Material* Object::GetMaterial() const
{
	return m_pMaterial;
}
