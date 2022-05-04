#include "InternalUtility/Rendering/Text.h"

namespace Rendering::Renderables
{
Text::Text()
	: Renderable(RenderObjectType::Text, Maths::vec2(0, 0), Color(255, 255, 255, 255))
	, m_scale(Maths::vec2(1, 1))
	, m_text("")
	, m_style(FontStyle::Regular)
{
}

Text::Text(Maths::vec2 position, Maths::vec2 scale, const std::string& text, const FontStyle& style,
	const Renderables::Color& color)
	: Renderable(RenderObjectType::Text, position, color)
	, m_scale(scale)
	, m_text(text)
	, m_style(style)
{
}

const std::string& Text::getText()
{
	return m_text;
}

const FontStyle& Text::getStyle()
{
	return m_style;
}

void Text::setStyle(const FontStyle& style)
{
	m_style = style;
}

const Maths::vec2& Text::getScale()
{
	return m_scale;
}

void Text::setScale(const Maths::vec2& scale)
{
	m_scale = scale;
}
}