#pragma once

#include <string>
#include <vector>

#include <Maths/vec2.h>

#include <InternalUtility/Rendering/Renderable.h>

namespace Rendering::Renderables
{

class MenuItem
{
	public:
		MenuItem(const std::string& name, const Maths::vec2& itemDimensions, const Color& fontColor);

		const Maths::vec2& getItemDimensions();
		const std::string& getName();
		const Color& getFontColor();

		void setName(const std::string& name);
		void setFontColor(const Color& color);

		virtual bool isSetting() = 0;

		template<class Type>
		Type* convertTo()
		{
			return (Type*)this;
		}

	private:
		std::string m_name;
		Maths::vec2 m_itemDimensions;
		Color m_fontColor;
};

class Menu : public MenuItem, public Renderable
{
	public:
		struct Setting : public MenuItem
		{
			public:
				Setting(const std::string& name, const Maths::vec2& itemDimensions, const Color& fontColor)
					: MenuItem(name, itemDimensions, fontColor)
				{
				}

				bool isSetting() override;

		};

		Menu(const std::string& name, const Maths::vec2& position, const Maths::vec2& itemDimensions, const Color& color, const Color& fontColor);

		Setting* addSetting(const std::string& settingName);
		Menu* addSubmenu(const std::string& menuName);

		Setting* getSetting(const std::string& name);
		std::vector<std::shared_ptr<MenuItem>>& getItems();
		Menu* getSubmenu(const std::string& name);
		std::vector<Menu*> getSubmenusRecursive();

		void setPosition(const Maths::vec2& position) override;
		bool isSetting() override;

	private:
		std::vector<std::shared_ptr<MenuItem>> m_items;
};
}