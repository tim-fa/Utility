#pragma once

#include "Renderable.h"

namespace Rendering::Renderables
{
struct BorderedRectangle : public Renderable
{
	public:
		BorderedRectangle(Maths::vec2 position, Maths::vec2 dimensions, const Renderables::Color& backgroundColor, const Renderables::Color& borderColor);
		BorderedRectangle();

		const Maths::vec2& getDimensions();

		void setDimensions(const Maths::vec2& dimensions);
		void setWidth(float w);
		void setHeight(float h);
		void setBorderColor(const Color& col);
		void setBorderThickness(float value);

		const Color& getBorderColor();
		float getBorderThickness();

	private:
		float m_borderThickness;
		Color m_borderColor;
		Maths::vec2 m_dimensions;
};
}


