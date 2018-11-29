#include <LinkNeuralNetwork.h>
#include <User Interface.h>
#include <SFML\Graphics.hpp>
#include <iomanip>
#include "utility.h"

#define iHIGH 1
#define iLOW 0

#define oHIGH 1
#define oLOW 0

namespace nn {

	template<uint tInputs, uint tHidden, uint tOutputs, bool tBias>
	void Draw(const nn::NeuralNetwork<tInputs, tHidden, tOutputs, tBias>& nn, sf::RenderWindow& window, const sf::Vector2f& sPos, const float& scaleX, const float& scaleY, const float& radius)
	{
		for (uint i = 0; i < tHidden + 2; i++)
		{
			for (uint j = 0; j < nn.GetStructure()[i] + (j != tHidden - 1 ? tBias : 0); j++)
			{
				sf::CircleShape circle(radius);
				circle.setOrigin(radius, radius);
				circle.setOutlineThickness(radius / 20);
				circle.setOutlineColor(sf::Color::White);
				circle.setFillColor(sf::Color::Black);

				sf::Vector2f pos1;

				pos1.x = i * scaleX;
				pos1.y = (j * scaleY) - (scaleY * ((nn.GetStructure()[i] - 1) / 2.f)) + (j == nn.GetStructure()[i] ? (scaleY / 3) : 0);

				circle.setPosition(pos1 + sPos);

				if (i != tHidden + 1)
				{
					for (uint k = 0; k < nn.GetStructure()[i + 1]; k++)
					{
						sf::Vector2f pos2;

						pos2.x = (i + 1) * scaleX;
						pos2.y = (k * scaleY) - (scaleY * ((nn.GetStructure()[i + 1] - 1) / 2.f)) + (k == nn.GetStructure()[i + 1] ? (scaleY / 3) : 0);

						ui::Line l("", pos1 + sPos, pos2 + sPos);

						uint index = k * nn.GetStructure()[i] + j;

						l.SetWidth(radius / 10);

						double color = tanh(nn.GetLinks()[i][index].weight);

						double r = (color < 0 ? 1 : 1 - color) * 255;
						double g = (color > 0 ? 1 : 1 + color) * 255;
						double b = (1 - abs(color)) * 255;

						l.SetColor({ (byte)r, (byte)g, (byte)b });
						l.Draw(window);
					}
				}

				window.draw(circle);
			}
		}
	}

}

Timer timer;
int main()
{
	RandINIT();

	std::array<std::array<double, 2>, 4> inputs = { { { iLOW, iLOW },{ iLOW, iHIGH },{ iHIGH, iLOW },{ iHIGH, iHIGH } } };
	std::array<std::array<double, 1>, 4> outputs = { { { oLOW },{ oHIGH },{ oHIGH },{ oLOW } } };

	nn::Activation sigmoid
	(
		"sigmoid",
		[](const double& x)->double { return (1.f / (1.f + exp(-x))) + (x / 10); },
		[](const double& x)->double { return ((1.f / (1.f + exp(-x))) * (1 - (1.f / (1.f + exp(-x))))) + (1 / 10); }
	);

	nn::NeuralNetwork<2, 2, 1, true> nn({ 3, 3 }, sigmoid);

	timer.Restart();
	for (uint i = 0; i < 2000; i++)
	{
		uint index = (rand() % (3 + 1));
		nn.Train(inputs[index], outputs[index], 0.3, 0.0);
	}
	std::cout << timer.GetElapsedTime<Timer::milliseconds>() << "\n";

	std::cout << std::fixed << std::setprecision(5) << nn.Calculate({ iLOW, iLOW })[0] << " | " << nn.Calculate({ iLOW, iHIGH })[0] << " | " << nn.Calculate({ iHIGH, iLOW })[0] << " | " << nn.Calculate({ iHIGH, iHIGH })[0] << "\n";

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window({ 1000, 700 }, "Test", sf::Style::Default, settings);

	nn.Calculate({ 0, 1 });

	window.clear(sf::Color::Black);
	nn::Draw(nn, window, { 100,  350 }, 200, 100, 20);
	window.display();

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}

			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::P)
				{
					sf::Texture tex;
					tex.create(window.getSize().x, window.getSize().y);
					tex.update(window);
					sf::Image img = tex.copyToImage();
					img.saveToFile("C:\\Users\\elies\\Desktop\\save.png");
				}
			}
		}
	}
}