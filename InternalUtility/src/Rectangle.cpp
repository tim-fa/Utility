#include "InternalUtility/Rendering/Rectangle.h"

namespace Renderables
{

Rectangle::Rectangle(Maths::vec2 position, Maths::vec2 dimensions, Maths::vec4 color)
	: Renderable(RenderObjectType::Rectangle, position, color)
	, m_dimensions(dimensions)
{
}

const Maths::vec2& Rectangle::getDimensions()
{
	return m_dimensions;
}

void Rectangle::setDimensions(const Maths::vec2& dimensions)
{
	m_dimensions = dimensions;
}

void Rectangle::setWidth(float w)
{
	m_dimensions.x = w;
}

void Rectangle::setHeight(float h)
{
	m_dimensions.y = h;
}
}
