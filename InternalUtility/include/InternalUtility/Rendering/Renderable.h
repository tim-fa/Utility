#pragma once

#include <Maths/vec2.h>
#include <Maths/vec4.h>

#include "Color.h"

namespace Rendering::Renderables
{
enum class RenderObjectType
{
		Rectangle,
		Text,
		Menu
};

class Renderable
{
	public:
		explicit Renderable(RenderObjectType type, Maths::vec2 position, const Renderables::Color& color);

		const Maths::vec2& getPosition();
		const Color& getColor();
		const RenderObjectType& getType();
		const bool& getEnabled() const;

		virtual void setPosition(const Maths::vec2& position);
		virtual void setXPos(float x);
		virtual void setYPos(float y);
		void setEnabled(bool enabled, bool applyForChildren = false);

	protected:
		Maths::vec2 m_position;
		Color m_color;
		RenderObjectType m_type;
		bool m_enabled;
};
}