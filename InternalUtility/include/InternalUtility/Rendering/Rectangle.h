#pragma once

#include "Renderable.h"

namespace Rendering::Renderables
{
struct Rectangle : public Renderable
{
	public:
		Rectangle(Maths::vec2 position, Maths::vec2 dimensions, const Renderables::Color& color);

		const Maths::vec2& getDimensions();

		void setDimensions(const Maths::vec2& dimensions);
		void setWidth(float w);
		void setHeight(float h);

	private:
		Maths::vec2 m_dimensions;
};
}


