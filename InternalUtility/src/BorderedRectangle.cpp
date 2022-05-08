#include "InternalUtility/Rendering/BorderedRectangle.h"

namespace Rendering::Renderables
{

BorderedRectangle::BorderedRectangle()
	: Renderable(RenderObjectType::BorderedRectangle, Maths::vec2(0, 0), Color(255, 255, 255, 50))
	, m_borderColor(Color(255, 0, 0, 255))
	, m_dimensions(50, 50)
	, m_borderThickness(1.0f)
{
}

BorderedRectangle::BorderedRectangle(Maths::vec2 position, Maths::vec2 dimensions, const Renderables::Color& backgroundColor, const Renderables::Color& borderColor)
	: Renderable(RenderObjectType::BorderedRectangle, position, backgroundColor)
	, m_borderColor(borderColor)
	, m_dimensions(dimensions)
	, m_borderThickness(1.0f)
{
}

const Maths::vec2& BorderedRectangle::getDimensions()
{
	return m_dimensions;
}

void BorderedRectangle::setDimensions(const Maths::vec2& dimensions)
{
	m_dimensions = dimensions;
}

void BorderedRectangle::setWidth(float w)
{
	m_dimensions.x = w;
}

void BorderedRectangle::setHeight(float h)
{
	m_dimensions.y = h;
}

void BorderedRectangle::setBorderColor(const Color& col)
{
	m_borderColor = col;
}

void BorderedRectangle::setBorderThickness(float value)
{
	m_borderThickness = value;
}

const Color& BorderedRectangle::getBorderColor()
{
	return m_borderColor;
}

float BorderedRectangle::getBorderThickness()
{
	return m_borderThickness;
}
}
