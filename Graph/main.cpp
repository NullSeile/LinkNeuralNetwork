#include <LinkNeuralNetwork.h>
#include <UITools/UITools.h>
#include <utility.h>
#include <fstream>

#define step 20.f
#define iter 5000.f

int main()
{
	RandINIT();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window({ 1200, 600 }, "Game", sf::Style::Default, settings);
	window.setFramerateLimit(144);

	nn::Activation sigmoid
	(
		"sigmoid",
		[](const double& x)->double { return (1.0 / (1.0 + exp(-x))) + (x / 20.0); },
		[](const double& x)->double { return ((1.0 / (1.0 + exp(-x))) * (1.0 - (1.0 / (1.0 + exp(-x))))) + (1.0 / 20.0); }
	);

	nn::NeuralNetwork<2, 2, 1> nn({ 4, 4 }, sigmoid, { -4, 4 });
	
	std::array<std::array<double, 2>, 4> inputs = { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } } };
	std::array<std::array<double, 1>, 4> outputs = { { { 0 }, { 1 }, { 1 }, { 0 } } };


	ui::Graph g("g");
	g.SetPosition({ 0, 0 });
	g.SetSize(window.getSize());

	std::vector<ui::Vec2f> points;

	//float value = 0.5f;
	//for (float x = 0; x < 100; x++)
	//{
	//	points.push_back({ x, value });
	//	value *= 3.5f * (1 - value);
	//}

	for (float x = 0; x < iter; x += step)
	{
		float sum = 0;
		for (int i = 0; i < step; i++)
		{
			uint index = (rand() % (3 + 1));
			sum += (float)nn.Train(inputs[index], outputs[index], 0.3);
		}

		points.push_back({ x, sum / step });
	}

	g.Plot(points, { 2, { 50, 50, 200 } });

	g.Fit();


	//nn.LoadFromFile("C:/Users/elies/Desktop/save.txt");

	//std::cout << nn.Calculate({ 0, 0 })[0] << " | " << nn.Calculate({ 0, 1 })[0] << " | " << nn.Calculate({ 1, 0 })[0] << " | " << nn.Calculate({ 1, 1 })[0] << "\n";

	
	//Timer t;
	while (window.isOpen())
	{
		ui::Event e;
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

			if (e.type == sf::Event::Resized)
			{
				ui::Vec2f size = window.getSize();

				sf::View view = window.getView();
				view.setSize(size);
				view.setCenter(size / 2.f);
				window.setView(view);

				g.SetSize(size);
			}

			//g.CheckInput(window, e);
		}

		window.clear();

		g.Update(window);
		g.Draw(window);

		window.display();
	}
}