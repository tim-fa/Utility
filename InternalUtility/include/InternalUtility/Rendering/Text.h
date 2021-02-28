#pragma once

#include <string>
#include "Renderable.h"

namespace Rendering::Renderables
{

enum class FontStyle{
		Regular,
		Bold
};

struct Text : public Renderable
{
	public:
		Text(Maths::vec2 position, Maths::vec2 scale, const std::string& text, const FontStyle& style, const Renderables::Color& color);

		const std::string& getText();
		const Maths::vec2& getScale();
		const FontStyle& getStyle();

		void setText(const std::string& text);
		void setScale(const Maths::vec2& scale);
		void setStyle(const FontStyle& style);

	private:
		std::string m_text;
		Maths::vec2 m_scale;
		FontStyle m_style;
};
}


