#include <LinkNeuralNetwork.h>
#include <UITools.h>
#include <utility.h>
#include <fstream>

#define step 10.f
#define iter 5000.f

int main()
{
	RandINIT();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window({ 1200, 600 }, "Game", sf::Style::Default, settings);

	nn::Activation sigmoid
	(
		"sigmoid",
		[](const double& x)->double { return (1.f / (1.f + exp(-x))) + (x / 20); },
		[](const double& x)->double { return ((1.f / (1.f + exp(-x))) * (1 - (1.f / (1.f + exp(-x))))) + (1 / 20); }
	);

	nn::NeuralNetwork<2, 2, 1> nn({ 4, 4 }, sigmoid, { -4, 4 });
	
	std::array<std::array<double, 2>, 4> inputs = { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } } };
	std::array<std::array<double, 1>, 4> outputs = { { { 0 }, { 1 }, { 1 }, { 0 } } };

	auto func = [&](const float& x)->float
	{
		float sum = 0;
		for (int i = 0; i < step; i++)
		{
			uint index = (rand() % (3 + 1));
			sum += (float)nn.Train(inputs[index], outputs[index], 0.3);
		}

		return sum / step;
	};

	ui::Graph g
	(
		"g",
		func,
		{ 0, iter },
		{ 0.f, 0.7f },
		step
	);
	g.SetLineWidth(1.5f);
	g.SetSize((sf::Vector2f)window.getSize());
	g.SetPosition({ 0, 0 });

	Timer t;
	g.Recalculate();
	std::cout << t.GetElapsedTime<Timer::milliseconds>() << "\n";

	//nn.LoadFromFile("C:/Users/elies/Desktop/save.txt");

	std::cout << nn.Calculate({ 0, 0 })[0] << " | " << nn.Calculate({ 0, 1 })[0] << " | " << nn.Calculate({ 1, 0 })[0] << " | " << nn.Calculate({ 1, 1 })[0] << "\n";

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

			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::P)
				{
					sf::Texture tex;
					tex.create(window.getSize().x, window.getSize().y);
					tex.update(window);
					sf::Image img = tex.copyToImage();
					img.saveToFile("C:/Users/elies/Desktop/save.png");
				}
				if (e.key.code == sf::Keyboard::S)
				{
					nn.SaveToFile("C:/Users/elies/Desktop/save.txt");
				}
			}
		}

		//window.clear();

		//window.display();
	}
}