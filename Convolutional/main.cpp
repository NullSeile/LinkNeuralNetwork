#pragma once

///////////////////////////////
//
// Convolutional test (not working)
//
///////////////////////////////


#include <iostream>
#include <vector>

typedef unsigned int uint;

namespace nn
{
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
}


#include <LinkNeuralNetwork.h>
#include <SFML\Graphics.hpp>

#define PI 3.1415926535
#define D_EPSILON 0.00000000000001

double HueToRGB(double arg1, double arg2, double H)
{
	if (H < 0) H += 1;
	if (H > 1) H -= 1;
	if ((6 * H) < 1) { return (arg1 + (arg2 - arg1) * 6 * H); }
	if ((2 * H) < 1) { return arg2; }
	if ((3 * H) < 2) { return (arg1 + (arg2 - arg1) * ((2.0 / 3.0) - H) * 6); }
	return arg1;
}

sf::Color HSVtoRGB(double h, double s, double v)
{
	///Reconvert to range [0,1]
	double H = h / 360.0;
	double S = s / 100.0;
	double L = v / 100.0;

	double arg1, arg2;

	if (S <= D_EPSILON)
	{
		sf::Color C((sf::Uint8)(L * 255), (sf::Uint8)(L * 255), (sf::Uint8)(L * 255));
		return C;
	}
	else
	{
		if (L < 0.5) { arg2 = L * (1 + S); }
		else { arg2 = (L + S) - (S * L); }
		arg1 = 2 * L - arg2;

		sf::Uint8 r = (sf::Uint8)(255 * HueToRGB(arg1, arg2, (H + 1.0 / 3.0)));
		sf::Uint8 g = (sf::Uint8)(255 * HueToRGB(arg1, arg2, H));
		sf::Uint8 b = (sf::Uint8)(255 * HueToRGB(arg1, arg2, (H - 1.0 / 3.0)));
		sf::Color C(r, g, b);
		return C;
	}
}

int get(int x, int y, int xMax)
{
	return x + xMax * y;
}

void ZoomAt(const sf::Vector2i& pos, sf::RenderWindow& window, const float& zoom)
{
	sf::Vector2f prev(window.mapPixelToCoords(pos));

	sf::View view(window.getView());
	view.zoom(zoom);
	window.setView(view);

	sf::Vector2f newPos(window.mapPixelToCoords(pos));
	sf::Vector2f delta(prev - newPos);
	view.move(delta);
	window.setView(view);
}

int main()
{
	float zoom = 1.f;
	std::vector<double> img;

	sf::Image imgT;

	std::string path;
	std::cin >> path;

	imgT.loadFromFile(path);

	for (uint y = 0; y < imgT.getSize().y; y++)
	{
		for (uint x = 0; x < imgT.getSize().x; x++)
		{
			sf::Color colorRGB = imgT.getPixel(x, y);
			float colorGRAY = (0.3f * colorRGB.r + 0.59f * colorRGB.g + 0.11f * colorRGB.b);

			img.push_back(nn::map(colorGRAY, 0, 255, -1, 1));
		}
	}

#if 0
	std::vector<double> filterY =
	{
		2,  2,  2,  2,  2,
		1,  1,  1,  1,  1,
		0,  0,  0,  0,  0,
		-1, -1, -1, -1, -1,
		-2, -2, -2, -2, -2
	};

	std::vector<double> filterX =
	{
		-2, -1,  0,  1,  2,
		-2, -1,  0,  1,  2,
		-2, -1,  0,  1,  2,
		-2, -1,  0,  1,  2,
		-2, -1,  0,  1,  2
	};
#else
	std::vector<double> filterY =
	{
		1,  1,  1,
		0,  0,  0,
		-1, -1, -1
	};

	std::vector<double> filterX =
	{
		-1,  0,  1,
		-1,  0,  1,
		-1,  0,  1
	};
#endif

	int n = (int)((sqrt(filterX.size()) - 1) / 2.f);

	sf::Vector2<std::vector<double>> newI;
	newI.x = nn::ApplyFilter(filterX, img, imgT.getSize().x, imgT.getSize().y);
	newI.y = nn::ApplyFilter(filterY, img, imgT.getSize().x, imgT.getSize().y);

	sf::RenderWindow window(
		sf::VideoMode((uint)((imgT.getSize().x - 2 * n) * zoom), (uint)((imgT.getSize().y - 2 * n) * zoom)),
		"",
		sf::Style::Close);

	sf::Vector2f bSize(window.getSize().x / (float)imgT.getSize().x, window.getSize().y / (float)imgT.getSize().y);

	sf::Image output;
	output.create(imgT.getSize().x - 2 * n, imgT.getSize().y - 2 * n);

	std::vector<sf::Vertex> m_pixels;
	for (uint x = 0; x < imgT.getSize().x - 2 * n; x++)
	{
		for (uint y = 0; y < imgT.getSize().y - 2 * n; y++)
		{
			double filtX = newI.x[get(x, y, imgT.getSize().x - 2 * n)];
			double filtY = newI.y[get(x, y, imgT.getSize().x - 2 * n)];

			float bright = (float)(sqrt(pow(filtX, 2) + pow(filtY, 2)));

			float angle = (float)(atan2(filtY, filtX) * 180 / PI);
			sf::Color c(HSVtoRGB(angle, 100, bright * 50));
			//sf::Uint8 col = (sf::Uint8)map<double>(filtX, -1, 1, 0, 255);
			//sf::Uint8 col = (sf::Uint8)map<double>(bright, 0, 1, 0, 255);
			//sf::Color c(col, col, col);

			output.setPixel(x, y, c);
		}
	}

	output.saveToFile("C:/Users/elies/Desktop/output.png");

	sf::Texture text;
	text.loadFromImage(output);
	sf::Sprite spr(text);
	spr.setScale(zoom, zoom);

	sf::Vector2f oldPos;

	bool pressed = false;

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
				if (e.key.code == sf::Keyboard::Space)
				{
					sf::View view = window.getView();
					view.reset(sf::FloatRect({ 0, 0 }, (sf::Vector2f)window.getSize()));
					window.setView(view);
				}
			}
			if (e.type == sf::Event::MouseWheelMoved)
			{
				if (e.mouseWheel.delta == 1)
				{
					ZoomAt(sf::Mouse::getPosition(window), window, 0.9f);
				}
				else if (e.mouseWheel.delta == -1)
				{
					ZoomAt(sf::Mouse::getPosition(window), window, 1.1f);
				}
			}
			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.mouseButton.button == sf::Mouse::Middle)
				{
					oldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					pressed = true;
				}
			}
			if (e.type == sf::Event::MouseMoved)
			{
				if (pressed)
				{
					sf::Vector2f newPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					sf::Vector2f delta = newPos - oldPos;

					sf::View view = window.getView();
					view.move(-delta);
					window.setView(view);

					oldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				}
			}
			if (e.type == sf::Event::MouseButtonReleased)
			{
				if (e.mouseButton.button == sf::Mouse::Middle)
				{
					pressed = false;
				}
			}
		}

		window.clear();

		window.draw(spr);

		window.display();
	}
}