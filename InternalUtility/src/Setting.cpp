#include "InternalUtility/Rendering/Menu.h"

int clamp(int value, int min, int max)
{
	return std::max(std::min(max, value), min);
}

namespace Utility
{

bool Setting::isSetting()
{
	return true;
}

void Setting::setValue(int value)
{
	int newVal = clamp(value, m_minValue, m_maxValue);
	if (m_valueChangedCallback && m_value != value) {
		m_valueChangedCallback(m_value, newVal);
	}
	m_value = newVal;
}

void Setting::increaseValue()
{
	int newVal = clamp(m_value + m_step, m_minValue, m_maxValue);
	if (m_valueChangedCallback && m_value != newVal) {
		m_valueChangedCallback(m_value, newVal);
	}
	m_value = newVal;
}

void Setting::decreaseValue()
{
	int newVal = clamp(m_value - m_step, m_minValue, m_maxValue);
	if (m_valueChangedCallback && m_value != newVal) {
		m_valueChangedCallback(m_value, newVal);
	}
	m_value = newVal;
}

void Setting::setMaxValue(int value)
{
	m_maxValue = value;
}

void Setting::setMinValue(int value)
{
	m_minValue = value;
}

void Setting::setStep(int value)
{
	m_step = value;
}

int Setting::getValue()
{
	return m_value;
}

int Setting::getMinValue()
{
	return m_minValue;
}

int Setting::getMaxValue()
{
	return m_maxValue;
}

int Setting::getStep()
{
	return m_step;
}
}