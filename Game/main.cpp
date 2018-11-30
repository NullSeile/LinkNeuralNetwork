#include <LinkNeuralNetwork.h>
#include <User Interface.h>
#include "utility.h"

/*class Grapha
{
private:
	std::vector<float> points;
	
	sf::Vector2f scale;

	sf::Vector2f pos;
	sf::Vector2f size;
	
public:
	Graph()
		: pos(0, 0)
		, size(100, 100)
		, scale(1, 1)
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
		for (uint i = 1; i < points.size(); i++)
		{
			ui::Line l("l", sf::Vector2f((i - 1) * scale.x, points[i - 1] * scale.y) + pos, sf::Vector2f(i * scale.x, points[i] * scale.y) + pos);
			l.SetColor(sf::Color::Black);

			l.Draw(window);
		}
	}
};*/

#define step 70
#define iter 40000

int main()
{
	RandINIT();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window({ 1200, 700 }, "Game", sf::Style::Default, settings);
		
	ui::Graph g("graph");
	g.SetPosition({ 0, 0 });
	g.SetSize({ 1200, 700 });
	g.SetRange({ { 0, iter }, { 0.7f, 0 } });
	g.SetStep(step);

	nn::Activation sigmoid
	(
		"sigmoid",
		[](const double& x)->double { return (1.f / (1.f + exp(-x))) + (x / 10); },
		[](const double& x)->double { return ((1.f / (1.f + exp(-x))) * (1 - (1.f / (1.f + exp(-x))))) + (1 / 10); }
	);

	nn::NeuralNetwork<2, 2, 1> nn({ 4, 4 }, sigmoid);
	

	std::array<std::array<double, 2>, 4> inputs = { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } } };
	std::array<std::array<double, 1>, 4> outputs = { { { 0 }, { 1 }, { 1 }, { 0 } } };

	uint point = 1;
	float sum = 0;
	for (uint i = 0; i < iter; i++)
	{
		uint index = (rand() % (3 + 1));
		sum += (float)nn.Train(inputs[index], outputs[index], 0.3, 0.0);

		if (i == (step * point) - 1)
		{
			g.AddData(sum / step);
			sum = 0;
			point++;
		}
	}

	std::cout << std::fixed << std::setprecision(5) << nn.Calculate({ 0, 0 })[0] << " | " << nn.Calculate({ 0, 1 })[0] << " | " << nn.Calculate({ 1, 0 })[0] << " | " << nn.Calculate({ 1, 1 })[0] << "\n";

	g.Draw(window);
	window.display();

	//Timer t;
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

		window.clear();

		g.Draw(window);

		window.display();
	}
}