#include "Sprite.h"

namespace ui {

	Sprite::Sprite(const std::string& id)
		: UIObject(id)
	{

	}

	void Sprite::Draw(sf::RenderWindow& window)
	{
		window.draw(*this);
	}

}