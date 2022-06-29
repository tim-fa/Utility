#include "InternalUtility/Rendering/Renderer.h"
#include "InternalUtility/Rendering/BorderedRectangle.h"
#include "fmt/format.h"
#include <chrono>
#include <thread>

using namespace Rendering::Renderables;

namespace Rendering
{

void Renderer::render()
{
	for (auto r : m_renderables) {
		if (!r || !r->getEnabled()) {
			continue;
		}
		auto type = r->getType();
		if (type == RenderObjectType::Rectangle) {
			auto rect = (Rectangle*)r;
			drawRect(rect->getPosition(), rect->getDimensions(), rect->getColor());
		} else if (type == RenderObjectType::BorderedRectangle) {
			auto rect = (BorderedRectangle*)r;
			rect->getPosition();
			rect->getBorderThickness();
			drawRect(rect->getPosition(), rect->getDimensions(), rect->getColor());
			drawRect(rect->getPosition() + Maths::vec2(0, 0), Maths::vec2(rect->getDimensions().x, rect->getBorderThickness()), rect->getBorderColor());
			drawRect(rect->getPosition() + Maths::vec2(0, rect->getDimensions().y), Maths::vec2(rect->getDimensions().x, rect->getBorderThickness()), rect->getBorderColor());
			drawRect(rect->getPosition() + Maths::vec2(0, 0), Maths::vec2(rect->getBorderThickness(), rect->getDimensions().y), rect->getBorderColor());
			drawRect(rect->getPosition() + Maths::vec2(rect->getDimensions().x, 0), Maths::vec2(rect->getBorderThickness(), rect->getDimensions().y), rect->getBorderColor());
		} else if (type == RenderObjectType::Text) {
			auto text = (Text*)r;
			drawString(text->getPosition(), text->getScale(), text->getText(), text->getStyle(), text->getColor());
		}
	}
}

void Renderer::clear()
{
	m_renderables.clear();
}
}