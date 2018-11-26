#pragma once

#include "Global.h"

namespace ui {

	class Interactuable
	{
	public:
		Interactuable();

		virtual void CheckInput(const sf::RenderWindow& window, const sf::Event& e) = 0;

		~Interactuable();
	};
}
