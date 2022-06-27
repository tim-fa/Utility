#pragma once

#include <string>
#include <vector>
#include <functional>

#include <Maths/vec2.h>

#include "Renderer.h"

namespace Utility
{
using namespace Rendering::Renderables;

class Menu;

class MenuItem
{
	public:
		MenuItem(const std::string& name, const Maths::vec2& itemDimensions, const Rendering::Color& fontColor, const Rendering::Color& selectionColor);

		const Maths::vec2& getItemDimensions();
		const std::string& getName();
		const Rendering::Color& getFontColor();
		const Rendering::Color& getSelectionColor();

		void setName(const std::string& name);
		void setFontColor(const Rendering::Color& color);
		void setSelectionColor(const Rendering::Color& color);

		virtual bool isSetting() = 0;

		class Menu* asMenu();
		struct Setting* asSetting();

	private:
		std::string m_name;
		Maths::vec2 m_itemDimensions;
		Rendering::Color m_fontColor;
		Rendering::Color m_selectionColor;
};

struct Setting : public MenuItem
{
	public:
		Setting(const std::string& name, int value, int minValue, int maxValue, int step, const std::function<void(int, int)>& onValueChanged,
			const Maths::vec2& itemDimensions, const Rendering::Color& fontColor, const Rendering::Color& selectionColor)
			: MenuItem(name, itemDimensions, fontColor, selectionColor)
			, m_value(value)
			, m_minValue(minValue)
			, m_maxValue(maxValue)
			, m_step(step)
			, m_valueChangedCallback(onValueChanged)
		{
		}

		void setValue(int value);
		void increaseValue();
		void decreaseValue();
		void setMaxValue(int value);
		void setMinValue(int value);
		void setStep(int value);

		int getValue();
		int getMinValue();
		int getMaxValue();
		int getStep();

		bool isSetting() override;
	private:
		int m_maxValue{};
		int m_minValue{};
		int m_step{};
		int m_value{};
		std::function<void(int oldValue, int newValue)> m_valueChangedCallback;
};

class Menu : public MenuItem
{
	public:


		Menu(const std::string& name, const Maths::vec2& position, const Maths::vec2& itemDimensions, const Rendering::Color& color, const Rendering::Color& fontColor,
			const Rendering::Color& selectionColor);

		Menu* getActiveMenu();
		void handleControls();

		Setting* addSetting(const std::string& settingName, int value = 0, int min = 0, int max = 1, int step = 1,
			const std::function<void(int oldVal, int newVal)>& valueChangedCallback = nullptr);
		Menu* addSubmenu(const std::string& menuName);

		Setting* getSetting(const std::string& name);
		Menu* getSubmenu(const std::string& name);
		MenuItem* getSelectedItem();
		std::vector<std::shared_ptr<MenuItem>>& getItems();
		std::vector<Menu*> getSubmenusRecursive();
		[[nodiscard]] int getSelectedItemIndex() const;

		void setPosition(const Maths::vec2& position);
		void setSelectedItemIndex(int index);
		void setEnabled(bool value);
		bool isSetting() override;
		bool getEnabled();

		void render(const Rendering::Renderer& renderer);

	private:
		bool m_enabled;
		Rendering::Color m_color;
		Maths::vec2 m_position;
		int m_selectedItemIndex;
		std::vector<std::shared_ptr<MenuItem>> m_items;
};
}