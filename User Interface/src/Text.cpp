#include "Text.h"

namespace ui {

	Text::Text(const std::string& id)
		: UIObject(id)
	{
	}

	void Text::Draw(sf::RenderWindow& window)
	{
		window.draw(*this);
	}

}