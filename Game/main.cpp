#include <LinkNeuralNetwork.h>
#include <User Interface.h>
#include "utility.h"

class Graph
{
private:
	std::vector<float> points;
	
	sf::Vector2f scale;
	
public:
	Graph(const sf::Vector2f& scale)
		: scale(scale)
	{

	}

	void SetScale(const sf::Vector2f& scale)
	{
		this->scale = scale;
	}

	void AddData(const float& y)
	{
		points.push_back(y);

		if (points.size() * scale.x > 1000)
		{
			points.erase(points.begin());
		}
	}

	void Draw(sf::RenderWindow& window, const sf::Vector2f& pos)
	{
		for (uint i = 0; i < points.size(); i++)
		{
			sf::CircleShape c(2);
			c.setOrigin(c.getRadius(), c.getRadius());
			c.setFillColor(sf::Color::Black);

			c.setPosition(sf::Vector2f(i * scale.x, points[i] * scale.y) + pos);

			window.draw(c);
		}
	}
};

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window({ 1200, 700 }, "Game", sf::Style::Default, settings);
	//window.setFramerateLimit(60);

	Graph g({ 10, 50 });

	float i = 0;
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::White);

		g.AddData(cos(i));
		g.Draw(window, { 10, 300 });
		i += 0.5;

		window.display();
	}
}