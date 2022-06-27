#pragma once

#include <string>
#include <fmt/format.h>
#include "Renderable.h"

namespace Rendering
{
enum class FontStyle
{
	Regular,
	Bold
};

namespace Renderables
{

struct Text : public Renderable
{
	public:
		Text(Maths::vec2 position, Maths::vec2 scale, const std::string& text, const FontStyle& style, const Color& color);
		Text();

		const std::string& getText();
		const Maths::vec2& getScale();
		const FontStyle& getStyle();

		template<class ... Args>
		void setText(const std::string& text, Args ... args)
		{
			m_text = fmt::format(text, args...);
		}

		void setScale(const Maths::vec2& scale);
		void setStyle(const FontStyle& style);

	private:
		std::string m_text;
		Maths::vec2 m_scale;
		FontStyle m_style;
};
}
}

