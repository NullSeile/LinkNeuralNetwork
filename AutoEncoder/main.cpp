#include <LinkNeuralNetwork.h>
#include <UITools\UITools.h>
#include <utility.h>
#include <filesystem>
#include <fstream>
#include <random>

#define step 10.f
#define iter 5000.f

int main()
{
	RandINIT(); 
	std::mt19937 randomGen(time(0));

	// Testing data
	//std::array<std::vector<std::array<double, 28 * 28>*>, 10> data;
	std::vector<std::array<double, 28 * 28>*> data;

	for (int n = 0; n < 10; n++)
	{
		std::string path = "D:\\Development\\Datasets\\mnist_png\\train\\";
		path += std::to_string(n);

		std::cout << path << "\n";

		int i = 0;
		for (auto& p : std::filesystem::directory_iterator(path))
		{
			sf::Image img;
			img.loadFromFile(p.path().string());

			int index = 0;
			auto arr = new std::array<double, 28 * 28>();
			for (uint y = 0; y < 28; y++)
			{
				for (uint x = 0; x < 28; x++)
				{
					(*arr)[index] = (double)img.getPixel(x, y).r * 255.f;
					index++;
				}
			}

			data.push_back(arr);
			//std::cout << "arr" << i << "\n";
			i++;

			if (i > 20)
				break;
		}
	}

	std::shuffle(data.begin(), data.end(), randomGen);

	nn::Activation sigmoid
	(
		"sigmoid",
		[](const double& x)->double { return (1.f / (1.f + exp(-x))); },
		[](const double& x)->double { return ((1.f / (1.f + exp(-x))) * (1 - (1.f / (1.f + exp(-x))))); }
	);

	//nn::NeuralNetwork<5, 5, 28 * 28> decoder({ 50, 100, 200, 300, 500 }, sigmoid);

	nn::NeuralNetwork<28 * 28, 9, 28 * 28> autoenc({ 500, 200, 100, 50, 20, 50, 100, 200, 500 }, nn::Activation::sigmoid );

	/*for (auto& input : data[0])
	{
		decoder.Train({ 1, 0, 0, 0, 0 }, *input, 0.05);
	}*/

	for (uint i = 0; i < 10; i++)
	{
		for (auto& input : data)
		{
			autoenc.Train(*input, *input, 0.05);
		}
	}
	auto output = autoenc.Calculate(*data[0]);

	sf::Image imgOut;
	imgOut.create(28, 28);
	int index = 0;
	for (uint j = 0; j < 28; j++)
	{
		for (uint i = 0; i < 28; i++)
		{
			std::cout << output[index] << "\n";
			float color = (output)[index] * 255.f;
			sf::Color c(color, color, color);

			imgOut.setPixel(i, j, c);
			index++;
		}
	}

	imgOut.saveToFile("C:/Users/elies/Desktop/out.png");

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window({ 800, 800 }, "Game", sf::Style::Default, settings);

	sf::Texture texture;
	texture.loadFromFile("C:/Users/elies/Desktop/out.png");

	sf::Sprite spr(texture);
	spr.setPosition(0, 0);
	spr.setScale(800 / 28.f, 800 / 28.f);

	window.draw(spr);
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
					img.saveToFile("C:/Users/elies/Desktop/save.png");
				}
			}

		}
	}
}