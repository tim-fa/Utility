#include "InternalUtility/Rendering/Renderable.h"

namespace Renderables
{

Renderable::Renderable(RenderObjectType type, Maths::vec2 position, Maths::vec4 color)
	: m_position(position)
	, m_color(color)
	, m_type(type)
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

const Maths::vec4& Renderable::getColor()
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
}
