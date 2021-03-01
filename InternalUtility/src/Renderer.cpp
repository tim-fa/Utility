#include "InternalUtility/Rendering/Renderer.h"
#include <fmt/format.h>

using namespace Rendering::Renderables;

namespace Rendering
{

void Renderer::render()
{
	for (auto& r : m_renderables) {
		if (!r->getEnabled()) {
			continue;
		}
		auto type = r->getType();
		if (type == RenderObjectType::Rectangle) {
			auto rect = (Rectangle*)r.get();
			drawRect(rect->getPosition(), rect->getDimensions(), rect->getColor());
		} else if (type == RenderObjectType::Text) {
			auto text = (Text*)r.get();
			drawString(text->getPosition(), text->getScale(), text->getText(), text->getStyle(), text->getColor());
		} else if (type == RenderObjectType::Menu) {
			auto menu = (Menu*)r.get();
			for (auto& submenu : menu->getSubmenusRecursive()) {
				if (!submenu->getEnabled()) {
					continue;
				}
				drawRect(submenu->getPosition(), Maths::vec2(submenu->getItemDimensions().x, submenu->getItemDimensions().y * submenu->getItems().size()),
					submenu->getColor());
				drawRect(
					Maths::vec2(submenu->getPosition().x, submenu->getPosition().y + submenu->getItemDimensions().y * (float)submenu->getSelectedItemIndex()),
					submenu->getItemDimensions(),
					submenu->getSelectionColor());
				int itemIndex = 1;
				for (auto& item : submenu->getItems()) {
					drawString(
						Maths::vec2(submenu->getPosition().x, submenu->getPosition().y + submenu->getItemDimensions().y * static_cast<float>(itemIndex)),
						Maths::vec2(1, 1),
						item->getName(),
						FontStyle::Regular,
						item->getFontColor());

					if (item->isSetting()) {
						drawString(
							Maths::vec2(submenu->getPosition().x + submenu->getItemDimensions().x - 70,
								submenu->getPosition().y + submenu->getItemDimensions().y * static_cast<float>(itemIndex)),
							Maths::vec2(1, 1),
							fmt::format("[{: 2d}/{: 2d}]", item->asSetting()->getValue(),
								item->asSetting()->getMaxValue()),
							FontStyle::Regular,
							item->getFontColor());
					}

					itemIndex++;
				}
			}
		}
	}
}

void Renderer::clear()
{
	m_renderables.clear();
}
}