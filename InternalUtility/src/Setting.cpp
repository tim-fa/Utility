#include "InternalUtility/Rendering/Menu.h"

int clamp(int value, int min, int max)
{
	return std::max(std::min(max, value), min);
}

namespace Rendering::Renderables
{

bool Menu::Setting::isSetting()
{
	return true;
}

void Menu::Setting::setValue(int value)
{
	int newVal = clamp(value, m_minValue, m_maxValue);
	if (m_valueChangedCallback) {
		m_valueChangedCallback(m_value, newVal);
	}
	m_value = newVal;
}

void Menu::Setting::increaseValue()
{
	int newVal = clamp(m_value + m_step, m_minValue, m_maxValue);
	if (m_valueChangedCallback) {
		m_valueChangedCallback(m_value, newVal);
	}
	m_value = newVal;
}

void Menu::Setting::decreaseValue()
{
	int newVal = clamp(m_value - m_step, m_minValue, m_maxValue);
	if (m_valueChangedCallback) {
		m_valueChangedCallback(m_value, newVal);
	}
	m_value = newVal;
}

void Menu::Setting::setMaxValue(int value)
{
	m_maxValue = value;
}

void Menu::Setting::setMinValue(int value)
{
	m_minValue = value;
}

void Menu::Setting::setStep(int value)
{
	m_step = value;
}

int Menu::Setting::getValue()
{
	return m_value;
}

int Menu::Setting::getMinValue()
{
	return m_minValue;
}

int Menu::Setting::getMaxValue()
{
	return m_maxValue;
}

int Menu::Setting::getStep()
{
	return m_step;
}
}