#pragma once

#include <vector>

#include <Maths/vec2.h>
#include <Maths/vec4.h>

#include "Renderable.h"
#include "Rectangle.h"
#include "Text.h"
#include "Color.h"
#include "Menu.h"

namespace Rendering
{
class Renderer
{
	public:
		template<class RenderableType>
		RenderableType* addRenderable(RenderableType* r)
		{
			m_renderables.push_back(r);
			return r;
		}

		template<class RenderableType>
		RenderableType* operator<<(RenderableType* r)
		{
			m_renderables.push_back(r);
			return r;
		}

		void render();
		void clear();

		virtual void drawRect(Maths::vec2 position, Maths::vec2 dimensions, const Renderables::Color& color) = 0;
		virtual void drawString(Maths::vec2 position, Maths::vec2 scale, const std::string& text, const Renderables::FontStyle& style, const Renderables::Color& color) = 0;

	private:
		std::vector<Renderables::Renderable*> m_renderables;
};
}

