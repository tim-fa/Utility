#pragma once

#include <Maths/vec2.h>
#include <Maths/vec4.h>

namespace Renderables
{
enum class RenderObjectType
{
		Rectangle
};

class Renderable
{
	public:
		explicit Renderable(RenderObjectType type, Maths::vec2 position, Maths::vec4 color);
		const RenderObjectType& getType();

		const Maths::vec2& getPosition();
		const Maths::vec4& getColor();

		void setPosition(const Maths::vec2& position);
		void setXPos(float x);
		void setYPos(float y);

	protected:
		Maths::vec2 m_position;
		Maths::vec4 m_color;
		RenderObjectType m_type;
};

}