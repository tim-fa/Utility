#include "InternalUtility/Rendering/Menu.h"

#include <fmt/format.h>
#include <Windows.h>

namespace Utility
{
MenuItem::MenuItem(const std::string& name, const Maths::vec2& itemDimensions, const Rendering::Color& fontColor, const Rendering::Color& selectionColor)
	: m_name(name)
	, m_itemDimensions(itemDimensions)
	, m_fontColor(fontColor)
	, m_selectionColor(selectionColor)
{
}

const Maths::vec2& MenuItem::getItemDimensions()
{
	return m_itemDimensions;
}

const Rendering::Color& MenuItem::getFontColor()
{
	return m_fontColor;
}

void MenuItem::setFontColor(const Rendering::Color& color)
{
	m_fontColor = color;
}

const std::string& MenuItem::getName()
{
	return m_name;
}

void MenuItem::setName(const std::string& name)
{
	m_name = name;
}

const Rendering::Color& MenuItem::getSelectionColor()
{
	return m_selectionColor;
}

void MenuItem::setSelectionColor(const Rendering::Color& color)
{
	m_selectionColor = color;
}

Menu* MenuItem::asMenu()
{
	if (isSetting()) {
		throw std::runtime_error(fmt::format("Cant convert menu setting '{}' to a menu", m_name));
	}
	return (Menu*)this;
}

Setting* MenuItem::asSetting()
{
	if (!isSetting()) {
		throw std::runtime_error(fmt::format("Cant menu menu '{}' to a setting", m_name));
	}
	return (Setting*)this;
}

Menu::Menu(const std::string& name, const Maths::vec2& position, const Maths::vec2& itemDimensions, const Rendering::Color& color,
	const Rendering::Color& fontColor, const Rendering::Color& selectionColor)
	: MenuItem(name, itemDimensions, fontColor, selectionColor)
	, m_selectedItemIndex(0)
	, m_color(color)
	, m_position(position)
{
}

Setting* Menu::addSetting(const std::string& settingName, int value, int min, int max, int step,
	const std::function<void(int oldVal, int newVal)>& valueChangedCallback)
{
	m_items.push_back(
		std::make_shared<Setting>(settingName, value, min, max, step, valueChangedCallback, getItemDimensions(), getFontColor(), getSelectionColor()));
	return (Setting*)m_items.back().get();
}

Menu* Menu::addSubmenu(const std::string& menuName)
{
	m_items.push_back(
		std::make_shared<Menu>(menuName, Maths::vec2(m_position.x + getItemDimensions().x, m_position.y), getItemDimensions(), m_color, getFontColor(),
			getSelectionColor()));
	m_items.back()->asMenu()->setEnabled(false);
	return m_items.back()->asMenu();
}

void Menu::setPosition(const Maths::vec2& position)
{
	m_position = position;
	for (auto& item: m_items) {
		if (!item->isSetting()) {
			item->asMenu()->setPosition(Maths::vec2(m_position.x + getItemDimensions().x, m_position.y));
		}
	}
}

Setting* Menu::getSetting(const std::string& name)
{
	for (auto& item: m_items) {
		if (item->isSetting()) {
			if (item->getName() == name) {
				return item->asSetting();
			}
		}
	}
	throw std::runtime_error(fmt::format("Requested menu item '{:s}' doesn't exist in menu '{:s}", name, getName()));
}

Menu* Menu::getSubmenu(const std::string& name)
{
	for (auto& item: m_items) {
		if (!item->isSetting()) {
			if (item->getName() == name) {
				return item->asMenu();
			}
		}
	}
	throw std::runtime_error(fmt::format("Requested submenu '{:s}' doesn't exist in menu '{:s}", name, getName()));
}

std::vector<Menu*> Menu::getSubmenusRecursive()
{
	std::vector<Menu*> result;
	result.push_back(asMenu());
	for (auto& item: m_items) {
		if (!item->isSetting()) {
			result.push_back(item->asMenu());
			auto tmp = item->asMenu()->getSubmenusRecursive();
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}
	return result;
}

std::vector<std::shared_ptr<MenuItem>>& Menu::getItems()
{
	return m_items;
}

bool Menu::isSetting()
{
	return false;
}

int Menu::getSelectedItemIndex() const
{
	return m_selectedItemIndex;
}

void Menu::setSelectedItemIndex(int index)
{
	if (index < 0) {
		index = 0;
	}
	if (index >= static_cast<int>(m_items.size())) {
		index = static_cast<int>(m_items.size()) - 1;
	}
	m_selectedItemIndex = index;
}

Menu* Menu::getActiveMenu()
{
	Menu* result = this;
	for (auto& menu: getSubmenusRecursive()) {
		if (menu->getEnabled()) {
			result = menu;
		}
	}
	return result;
}

MenuItem* Menu::getSelectedItem()
{
	return m_items.at(m_selectedItemIndex).get();
}

void Menu::handleControls()
{
	auto currentMenu = getActiveMenu();
	if (GetAsyncKeyState(VK_DELETE) & 1) {
		setEnabled(!getEnabled());
	}
	if (getEnabled()) {
		if (GetAsyncKeyState(VK_DOWN) & 1) {
			currentMenu->setSelectedItemIndex(currentMenu->getSelectedItemIndex() + 1);
		}
		if (GetAsyncKeyState(VK_UP) & 1) {
			currentMenu->setSelectedItemIndex(currentMenu->getSelectedItemIndex() - 1);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 1) {
			if (!currentMenu->getSelectedItem()->isSetting()) {
				currentMenu->getSelectedItem()->asMenu()->setEnabled(true);
			} else {
				currentMenu->getSelectedItem()->asSetting()->increaseValue();
			}
		}
		if (GetAsyncKeyState(VK_LEFT) & 1 && currentMenu->getSelectedItem()->isSetting()) {
			currentMenu->getSelectedItem()->asSetting()->decreaseValue();
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 1) {
			currentMenu->setEnabled(false);
		}
	}
}

void Menu::render(const Rendering::Renderer& renderer)
{
	for (auto& submenu: getSubmenusRecursive()) {
		if (!submenu->getEnabled()) {
			continue;
		}
		renderer.drawRect(submenu->m_position, Maths::vec2(submenu->getItemDimensions().x, submenu->getItemDimensions().y * submenu->getItems().size()),
			submenu->m_color);
		renderer.drawRect(
			Maths::vec2(submenu->m_position.x, submenu->m_position.y + submenu->getItemDimensions().y * (float)submenu->getSelectedItemIndex()),
			submenu->getItemDimensions(),
			submenu->getSelectionColor());
		int itemIndex = 1;
		for (auto& item: submenu->getItems()) {
			renderer.drawString(
				Maths::vec2(submenu->m_position.x, submenu->m_position.y + submenu->getItemDimensions().y * static_cast<float>(itemIndex)),
				Maths::vec2(.2),
				item->getName(),
				Rendering::FontStyle::Regular,
				item->getFontColor());

			std::string itemOption;
			if (item->isSetting()) {
				itemOption = fmt::format("[{: 2d}/{: 2d}]", item->asSetting()->getValue(),
					item->asSetting()->getMaxValue());
			} else {
				itemOption = "[->]";
			}
			renderer.drawString(
				Maths::vec2(submenu->m_position.x + submenu->getItemDimensions().x - 70,
					submenu->m_position.y + submenu->getItemDimensions().y * static_cast<float>(itemIndex)),
				Maths::vec2(.2),
				itemOption,
				Rendering::FontStyle::Regular,
				item->getFontColor());
			itemIndex++;
		}
	}
}

bool Menu::getEnabled()
{
	return m_enabled;
}

void Menu::setEnabled(bool value)
{
	m_enabled = value;
}
}