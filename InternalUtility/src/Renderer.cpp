#include "InternalUtility/Rendering/Renderer.h"

using namespace Renderables;

namespace Renderer
{

void Renderer::render()
{
	for (auto& r : m_renderables) {
		switch (r->getType()) {
			case RenderObjectType::Rectangle:
				auto rect = (Rectangle*)r.get();
				drawRect(rect->getPosition(), rect->getDimensions(), rect->getColor());
				break;
		}
	}
}

void Renderer::clear()
{
	m_renderables.clear();
}

}