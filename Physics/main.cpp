#include <LinkNeuralNetwork.h>
#include <User Interface.h>
#include <utility.h>
#include "literals.h"

#define sX 1200
#define sY 600

#define vY 20_m
#define vX ((sX / sY) * vY)

#define bot 0

double Normalize(const double& x)
{
	return (tanh(x) + 1) / 2.;
}

class Player
{
public:
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Vector2f acc;
	float rotation;
	const float mass = 2;

	sf::ConvexShape shape;

	Player()
	{
		Reset();

		float r = 1_m;

		shape.setPointCount(4);

		shape.setPoint(0, { (float)(r * 1.5 * cos(0 * ((2 * PI) / 3))),
							(float)(r *		 sin(0 * ((2 * PI) / 3))) });

		shape.setPoint(1, { (float)(r * 1.5 * cos(1 * ((2 * PI) / 3))),
							(float)(r *		 sin(1 * ((2 * PI) / 3))) });

		shape.setPoint(2, { -r * 0.5f, 0 });

		shape.setPoint(3, { (float)(r * 1.5 * cos(2 * ((2 * PI) / 3))),
							(float)(r *		 sin(2 * ((2 * PI) / 3))) });

		shape.setFillColor(sf::Color::Black);
		shape.setPosition(pos);
		shape.setRotation(rotation);
	}

	void Reset()
	{
		pos = { 0, 10 };
		vel = { 0, 2 };
		acc = { 0, 0 };
		rotation = 90;
	}

	float GetVel()
	{
		return sqrt(pow(vel.x, 2) + pow(vel.y, 2));
	}

	void Update(const float& dt)
	{
		acc.y += -9.8f;
		vel += acc * dt;
		pos += (vel * dt) + (0.5f * acc * dt * dt);

		shape.setPosition(pos);
		shape.setRotation(rotation);

		acc = { 0, 0 };
	}

	void ApplyForce(const sf::Vector2f& force)
	{
		acc += force / mass;
	}

	void ApplyForce(const float& force, const float& angle)
	{
		acc.x += (force * cos(ToRad(angle))) / mass;
		acc.y += (force * sin(ToRad(angle))) / mass;
	}

	void Rotate(const float& deltaAngle)
	{
		rotation += deltaAngle;
	}

	void Draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}
};

struct Data
{
	sf::Vector2f pPos;
	sf::Vector2f pVel;
	float bX;
	bool up;
	bool left;
	bool right;

	Data(const sf::Vector2f& pPos, const sf::Vector2f& pVel, const float& bX, bool up = false, bool left = false, bool right = false)
		: pPos(pPos)
		, pVel(pVel)
		, bX(bX)
		, up(up)
		, left(left)
		, right(right)
	{
	}

	std::array<double, 5> Inputs()
	{
		std::array<double, 5> out;
		out[0] = (double)(pPos.x / vX);
		out[1] = (double)(pPos.y / vY);
		out[2] = Normalize((double)pVel.x);
		out[3] = Normalize((double)pVel.y);
		out[4] = (float)(bX / vX);

		return out;
	}

	std::array<double, 3> Ouputs()
	{
		std::array<double, 3> out;
		out[0] = up;
		out[1] = left;
		out[2] = right;

		return out;
	}
};

void Restart(Player& p, sf::RectangleShape& base, std::vector<Data>& data)
{
	p.Reset();
	base.setPosition((float)randRange(-vX / 2.f, (vX / 2.f) - base.getSize().x), 0);
	data.clear();
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window({ sX, sY }, "Game", sf::Style::Close, settings);
	window.setFramerateLimit(30);

	sf::View view = window.getView();
	view.setCenter(0, vY / 2.f);
	view.setSize(vX, -vY);
	window.setView(view);

	std::array<sf::RectangleShape, 4> walls;

	walls[0].setSize({ vX, -10_cm });
	walls[0].setPosition(-vX / 2.f, vY);
	walls[0].setFillColor(sf::Color::Black);

	walls[1].setSize({ vX, 10_cm });
	walls[1].setPosition(-vX / 2.f, 0);
	walls[1].setFillColor(sf::Color::Black);

	walls[2].setSize({ 10_cm, vY });
	walls[2].setPosition(-vX / 2.f, 0);
	walls[2].setFillColor(sf::Color::Black);

	walls[3].setSize({ -10_cm, vY });
	walls[3].setPosition(vX / 2.f, 0);
	walls[3].setFillColor(sf::Color::Black);

	Player p;

	sf::RectangleShape base;
	base.setSize({ 4_m, 50_cm });
	base.setFillColor(sf::Color::Blue);

	nn::Activation sigmoid
	(
		"sigmoid",
		[](const double& x)->double { return (1.f / (1.f + exp(-x))) + (x / 10); },
		[](const double& x)->double { return ((1.f / (1.f + exp(-x))) * (1 - (1.f / (1.f + exp(-x))))) + (1 / 10); }
	);

	nn::NeuralNetwork<5, 3, 3> nn({ 6, 6, 6 }, sigmoid, { -5, 5 });

#if bot == 1
	nn.LoadFromFile("C:/Users/elies/Desktop/save.txt");
#endif
	std::vector<Data> data;

	Restart(p, base, data);

	float dt = 0;
	Timer t;
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				nn.SaveToFile("C:/Users/elies/Desktop/save.txt");
				window.close();
			}
		}

#if bot == 0
		bool left = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			left = true;
			p.Rotate(150 * dt);
		}
		bool right = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			right = true;
			p.Rotate(-150 * dt);
		}
		bool up = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			up = true;
			p.ApplyForce(50, p.rotation);
		}

		data.push_back(Data(p.pos, p.vel, base.getPosition().x, up, left, right));
#else
		std::array<double, 3> input = nn.Calculate(Data(p.pos, p.vel, base.getPosition().x).Inputs());
		
		std::cout << "Up: " << input[0] << " | Left: " << input[1] << " | Right: " << input[2] << "\n";

		if (input[1] > 0.5) // Left
			p.Rotate(150 * dt);

		if (input[2] > 0.5) // Right
			p.Rotate(-150 * dt);

		if (input[0] > 0.5) // Up
			p.ApplyForce(50, p.rotation);
#endif
		window.clear(sf::Color::White);

		window.draw(base);
		if (p.shape.getGlobalBounds().intersects(base.getGlobalBounds()))
		{
			if (p.GetVel() < 5 && (p.rotation > 60 && p.rotation < 120))
			{
				for (Data& d : data)
				{
					nn.Train(d.Inputs(), d.Ouputs(), 0.05, 0.0);
				}

				Restart(p, base, data);

				window.clear();
			}
			else
			{
				Restart(p, base, data);
			}
		}

		for (sf::RectangleShape& w : walls)
		{
			window.draw(w);

			if (p.shape.getGlobalBounds().intersects(w.getGlobalBounds()))
			{
				Restart(p, base, data);
			}
		}

		p.Update(dt);
		p.Draw(window);

		window.display();

		dt = (float)t.GetElapsedTime<Timer::seconds>();
		t.Restart();
	}
}