#pragma once

#include "UIObject.h"

namespace ui {

	class Graph : public ui::UIObject
	{
	private:
		sf::RectangleShape m_backgrownd;
		std::vector<float> m_values;
		sf::Vector2<sf::Vector2f> m_range;
		float m_lineWidth;
		float m_step;

	public:
		Graph(const std::string& id);

		void SetPosition(const sf::Vector2f& pos);
		void SetSize(const sf::Vector2f& size);
		void SetRange(const sf::Vector2<sf::Vector2f>& range);
		void SetStep(const float& step);
		void SetLineWidth(const float& width);

		void AddData(const float& value);
		void Draw(sf::RenderWindow& window) override;

		~Graph();
	};

}