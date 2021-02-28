#include "InternalUtility/Rendering/Renderer.h"

using namespace Rendering::Renderables;

namespace Rendering
{

void Renderer::render()
{
	for (auto& r : m_renderables) {
		auto type = r->getType();
		if (type == RenderObjectType::Rectangle) {
			auto rect = (Rectangle*)r.get();
			drawRect(rect->getPosition(), rect->getDimensions(), rect->getColor());
		} else if (type == RenderObjectType::Text) {
			auto text = (Text*)r.get();
			drawString(text->getPosition(), text->getScale(), text->getText(), text->getStyle(), text->getColor());
		}
	}
}

void Renderer::clear()
{
	m_renderables.clear();
}
}