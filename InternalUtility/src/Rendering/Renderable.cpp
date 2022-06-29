#include "InternalUtility/Rendering/Renderable.h"

namespace Rendering::Renderables
{

Renderable::Renderable(RenderObjectType type, Maths::vec2 position, const Color& color)
	: m_position(position)
	, m_color(color)
	, m_type(type)
	, m_enabled(true)
{
}

const RenderObjectType& Renderable::getType()
{
	return m_type;
}

const Maths::vec2& Renderable::getPosition()
{
	return m_position;
}

const Color& Renderable::getColor()
{
	return m_color;
}

void Renderable::setPosition(const Maths::vec2& position)
{
	m_position = position;
}

void Renderable::setXPos(float x)
{
	m_position.x = x;
}

void Renderable::setYPos(float y)
{
	m_position.y = y;
}

const bool& Renderable::getEnabled() const
{
	return m_enabled;
}

void Renderable::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

void Renderable::setColor(const Color& color)
{
	m_color = color;
}
}
