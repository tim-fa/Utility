#include "InternalUtility/Rendering/Menu.h"

#include <fmt/format.h>

namespace Rendering::Renderables
{

MenuItem::MenuItem(const std::string& name, const Maths::vec2& itemDimensions, const Color& fontColor)
	: m_name(name)
	, m_itemDimensions(itemDimensions)
	, m_fontColor(fontColor)
{
}

const Maths::vec2& MenuItem::getItemDimensions()
{
	return m_itemDimensions;
}

const Color& MenuItem::getFontColor()
{
	return m_fontColor;
}

void MenuItem::setFontColor(const Color& color)
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

bool Menu::Setting::isSetting()
{
	return true;
}

Menu::Menu(const std::string& name, const Maths::vec2& position, const Maths::vec2& itemDimensions, const Color& color, const Color& fontColor)
	: Renderable(RenderObjectType::Menu, position, color)
	, MenuItem(name, itemDimensions, fontColor)
{
}

Menu::Setting* Menu::addSetting(const std::string& settingName)
{
	m_items.push_back(std::make_shared<Setting>(settingName, getItemDimensions(), getFontColor()));
	return (Menu::Setting*)m_items.back().get();
}

Menu* Menu::addSubmenu(const std::string& menuName)
{
	m_items.push_back(
		std::make_shared<Menu>(menuName, Maths::vec2(m_position.x + getItemDimensions().x, m_position.y), getItemDimensions(), getColor(), getFontColor()));
	return m_items.back()->convertTo<Menu>();
}

void Menu::setPosition(const Maths::vec2& position)
{
	m_position = position;
	for (auto& item : m_items) {
		if (!item->isSetting()) {
			item->convertTo<Menu>()->setPosition(Maths::vec2(m_position.x + getItemDimensions().x, m_position.y));
		}
	}
}

Menu::Setting* Menu::getSetting(const std::string& name)
{
	for (auto& item : m_items) {
		if (item->isSetting()) {
			if (item->getName() == name) {
				return item->convertTo<Setting>();
			}
		}
	}
	throw std::runtime_error(fmt::format("Requested menu item '{:s}' doesn't exist in menu '{:s}", name, getName()));
}

Menu* Menu::getSubmenu(const std::string& name)
{
	for (auto& item : m_items) {
		if (!item->isSetting()) {
			if (item->getName() == name) {
				return item->convertTo<Menu>();
			}
		}
	}
	throw std::runtime_error(fmt::format("Requested submenu '{:s}' doesn't exist in menu '{:s}", name, getName()));
}

std::vector<Menu*> Menu::getSubmenusRecursive()
{
	std::vector<Menu*> result;
	result.push_back(convertTo<Menu>());
	for (auto& item : m_items) {
		if (!item->isSetting()) {
			result.push_back(item->convertTo<Menu>());
			auto tmp = item->convertTo<Menu>()->getSubmenusRecursive();
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
}
