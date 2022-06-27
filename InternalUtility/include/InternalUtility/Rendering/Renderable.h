#pragma once

#include <Maths/vec2.h>
#include <Maths/vec4.h>

#include "Color.h"

namespace Rendering::Renderables
{
enum class RenderObjectType
{
	Rectangle = 0x1,
	BorderedRectangle = 0x2,
	Text = 0x3,
	Menu = 0x4
};

class Renderable
{
	public:
		explicit Renderable(RenderObjectType type, Maths::vec2 position, const Color& color);

		const Maths::vec2& getPosition();
		const Color& getColor();
		const RenderObjectType& getType();
		const bool& getEnabled() const;

		virtual void setColor(const Color& color);
		virtual void setPosition(const Maths::vec2& position);
		virtual void setXPos(float x);
		virtual void setYPos(float y);
		void setEnabled(bool enabled);

	protected:
		Maths::vec2 m_position;
		Color m_color;
		RenderObjectType m_type;
		bool m_enabled;
};
}