#pragma once

#include <vector>

#include <Maths/vec2.h>
#include <Maths/vec4.h>

#include "Renderable.h"
#include "Rectangle.h"
#include "Text.h"
#include "Color.h"

namespace Rendering
{
class Renderer
{
	public:
		void addRenderable(Renderables::Renderable* r)
		{
			m_renderables.push_back(r);
		}

		void operator<<(Renderables::Renderable* r)
		{
			m_renderables.push_back(r);
		}

		void render();
		void clear();

		virtual void drawRect(Maths::vec2 position, Maths::vec2 dimensions, const Color& color) const = 0;
		virtual void drawString(Maths::vec2 position, Maths::vec2 scale, const std::string& text, const FontStyle& style, const Color& color) const = 0;

	private:
		std::vector<Renderables::Renderable*> m_renderables;
};
}

