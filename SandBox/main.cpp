#include <LinkNeuralNetwork.h>
#include <UITools.h>
#include <SFML\Graphics.hpp>
#include <iomanip>
#include "utility.h"

#define iHIGH 1
#define iLOW 0

#define oHIGH 1
#define oLOW 0




namespace nn {

	template<uint tInputs, uint tHidden, uint tOutputs, bool tBias>
	void Draw(const nn::NeuralNetwork<tInputs, tHidden, tOutputs, tBias>& nn, sf::RenderWindow& window, const ui::Vec2f& sPos, const float& scaleX, const float& scaleY, const float& radius)
	{
		sf::Color mColor = { 165, 200, 225 };

		for (uint i = 0; i < tHidden + 2; i++)
		{
			for (uint j = 0; j < nn.GetStructure()[i] + (j != tHidden - 1 ? tBias : 0); j++)
			{
				sf::CircleShape circle(radius);
				circle.setOrigin(radius, radius);
				circle.setOutlineThickness(radius / 4);
				//circle.setOutlineColor(mColor);
				circle.setOutlineColor(sf::Color::Black);
				//float c = (float)nn::map((double)nn.GetNeurons()[i][j].value, 0, 1, 0, 255);
				float c = 170;
				//std::cout << nn.GetNeurons()[i][j].error << "\n";
				circle.setFillColor(sf::Color(c, c, c));
				//circle.setFillColor(sf::Color::White);

				sf::Vector2f pos1;

				pos1.x = i * scaleX;
				pos1.y = (j * scaleY) - (scaleY * ((nn.GetStructure()[i] - 1) / 2.f)) + (j == nn.GetStructure()[i] ? (scaleY / 3) : 0);

				circle.setPosition(pos1 + sPos);

				if (i != tHidden + 1)
				{
					for (uint k = 0; k < nn.GetStructure()[i + 1]; k++)
					{
						ui::Vec2f pos2;

						pos2.x = (i + 1) * scaleX;
						pos2.y = (k * scaleY) - (scaleY * ((nn.GetStructure()[i + 1] - 1) / 2.f)) + (k == nn.GetStructure()[i + 1] ? (scaleY / 3) : 0);

						ui::Line l("", pos1 + sPos, pos2 + sPos);

						uint index = k * nn.GetStructure()[i] + j;

						float w = (tanh(nn.GetLinks()[i][index].weight) + 3) / 7.f;

						//l.SetWidth(randRange(5, 10));

						l.SetWidth(radius / 8.f);

						//double color = tanh(nn.GetLinks()[i][index].weight);

						//double r = (color < 0 ? 1 : 1 - color) * 255;
						//double g = (color > 0 ? 1 : 1 + color) * 255;
						//double b = (1 - abs(color)) * 255;

						//l.SetColor({ (byte)r, (byte)g, (byte)b });

						//l.SetColor(mColor);
						l.SetColor(sf::Color::Black);
						l.Draw(window);
					}
				}

				window.draw(circle);
			}
		}
	}

}

// Applies a filter to an image. It reduces a pixel the output image
std::vector<double> ApplyFilter(const std::vector<double>& filter, const std::vector<double> source, const uint& sourceX, const uint& sourceY)
{
	auto get = [](const int& x, const int& y, const int& xMax)->int
	{
		return x + xMax * y;
	};

	std::vector<double> newI;

	int fMaxX = (int)sqrt(filter.size());

	int n = (fMaxX - 1) / 2;

	double total = 0;
	for (const double& i : filter)
	{
		total += abs(i);
	}

	for (uint y = n; y < sourceY - n; y++)
	{
		for (uint x = n; x < sourceX - n; x++)
		{
			double sum = 0;
			for (int i = -n; i <= n; i++)
			{
				for (int j = -n; j <= n; j++)
				{
					sum += source[get(x + i, y + j, sourceX)] * filter[get(i + n, j + n, fMaxX)];
				}
			}
			newI.push_back(sum / total);
		}
	}

	return newI;
}


int main()
{
	RandINIT();
	
	Timer t;

	//sf::ContextSettings settings;
	//settings.antialiasingLevel = 16;

	//sf::RenderWindow window({ 1920, 1080}, "Test", sf::Style::Default, settings);


	//window.clear({217, 217, 217});
	//window.clear(sf::Color::White);
	//nn::Draw(nn, window, { 200, 520 }, 350, 120, 25);

	//window.display();
	std::array<std::array<double, 2>, 4> inputs =	{ { { 0, 0 },	{ 0, 1 },	{ 1, 0 },	{ 1, 1 }	} };
	std::array<std::array<double, 1>, 4> outputs =	{ { { 0 },		{ 1 },		{ 1 },		{ 0 }		} };

	nn::NeuralNetwork<2, 2, 1, false> nn({ 500, 500 }, nn::Activation::sigmoid);

	const int steps = 1000;
	float sum;
	
	sum = 0;
	for (int i = 0; i < steps; i++)
	{
		uint index = (rand() % (3 + 1));

		t.Restart();
		nn.Train(inputs[index], outputs[index], 0.3, 0.0, true);
		sum += t.GetElapsedTime<Timer::milliseconds>();
	}
	std::cout << "Async: " << sum / (float)steps << "\n";

	//sum = 0;
	//for (int i = 0; i < steps; i++)
	//{
	//	uint index = (rand() % (3 + 1));

	//	t.Restart();
	//	nn.Train(inputs[index], outputs[index], 0.3, 0.0, false);
	//	sum += t.GetElapsedTime<Timer::milliseconds>();
	//}
	//std::cout << "Squen: " << sum / (float)steps << "\n";


	/*while (window.isOpen())
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

			//window.clear();

			//nn::Draw(nn, window, { 70, 350 }, 150, 70, 20);

			//uint index = (rand() % (3 + 1));
			//nn.Train(inputs[index], outputs[index], 0.1);

			//window.display();
		}
	}*/
}