#include "InternalUtility/Rendering/Renderer.h"
#include "InternalUtility/Rendering/BorderedRectangle.h"
#include <fmt/format.h>
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
		} else if (type == RenderObjectType::Menu) {
			auto menu = (Menu*)r;
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

					std::string itemOption;
					if (item->isSetting()) {
						itemOption = fmt::format("[{: 2d}/{: 2d}]", item->asSetting()->getValue(),
							item->asSetting()->getMaxValue());
					} else {
						itemOption = "[->]";
					}
					drawString(
						Maths::vec2(submenu->getPosition().x + submenu->getItemDimensions().x - 70,
							submenu->getPosition().y + submenu->getItemDimensions().y * static_cast<float>(itemIndex)),
						Maths::vec2(1, 1),
						itemOption,
						FontStyle::Regular,
						item->getFontColor());
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