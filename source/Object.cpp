#include "Object.h"

Object::Object(const Elite::FPoint3& position, const Elite::RGBColor& color)
	: m_Position{position}
	, m_Color{color}
{
	m_Color.MaxToOne();
}

void Object::SetPosition(const Elite::FPoint3& position)
{
	m_Position = position;
}
const Elite::FPoint3& Object::GetPosition()
{
	return m_Position;
}

void Object::SetColor(const Elite::RGBColor& color)
{
	m_Color = color;
	m_Color.MaxToOne();
}
const Elite::RGBColor& Object::GetColor()
{
	return m_Color;
}