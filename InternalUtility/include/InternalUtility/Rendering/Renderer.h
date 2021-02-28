#pragma once

#include <vector>

#include <Maths/vec2.h>
#include <Maths/vec4.h>

#include "Renderable.h"
#include "Rectangle.h"

namespace Renderer
{
class Renderer
{
	public:
		template<class RenderableType>
		RenderableType* addRenderable(RenderableType&& r)
		{
			m_renderables.push_back(std::make_shared<RenderableType>(r));
			return (RenderableType*)m_renderables.back().get();
		}
		template<class RenderableType>
		void addRenderable(RenderableType& r)
		{
			m_renderables.push_back(std::make_shared<RenderableType>(r));
			return (RenderableType*)m_renderables.back().get();
		}
		void render();
		void clear();

		virtual void drawRect(Maths::vec2 position, Maths::vec2 dimensions, Maths::vec4 color) = 0;

	private:
		std::vector<std::shared_ptr<Renderables::Renderable>> m_renderables;
};
}

