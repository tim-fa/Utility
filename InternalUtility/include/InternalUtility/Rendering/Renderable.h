#pragma once

#include <Maths/vec2.h>
#include <Maths/vec4.h>

#include "Color.h"

namespace Rendering::Renderables
{
enum class RenderObjectType
{
		Rectangle
};

class Renderable
{
	public:
		explicit Renderable(RenderObjectType type, Maths::vec2 position, const Renderables::Color& color);
		const RenderObjectType& getType();

		const Maths::vec2& getPosition();
		const Color& getColor();

		void setPosition(const Maths::vec2& position);
		void setXPos(float x);
		void setYPos(float y);

	protected:
		Maths::vec2 m_position;
		Color m_color;
		RenderObjectType m_type;
};

}