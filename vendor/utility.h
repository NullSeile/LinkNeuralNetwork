#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>
#include <execution>

#define RandINIT() srand((unsigned int)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

typedef unsigned int uint;
typedef unsigned char byte;

using namespace std::chrono_literals;

#define WaitFor(x) std::this_thread::sleep_for(x)

class Timer
{
private:
	std::chrono::steady_clock::time_point start;

public:
	Timer()
	{
		Restart();
	}

	using nanoseconds = std::chrono::duration<double, std::nano>;
	using microseconds = std::chrono::duration<double, std::micro>;
	using milliseconds = std::chrono::duration<double, std::milli>;
	using seconds = std::chrono::duration<double>;
	using minutes = std::chrono::duration<double, std::ratio<60>>;
	using hours = std::chrono::duration<double, std::ratio<3600>>;

	void Restart()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	template<class T>
	double GetElapsedTime()
	{
		return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - start).count();
	}
};

inline double map(const double& value, const double& inputMin, const double& inputMax, const double& outputMin, const double& outputMax)
{
	return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (value - inputMin);
}

inline double randRange(const double& min, const double& max)
{
	return min + (rand() / (RAND_MAX / ((max + 0) - min)));
}

inline float randRange(const float& min, const float& max)
{
	return min + (rand() / (RAND_MAX / ((max + 0) - min)));
}

template<typename F>
inline void for_async(int begin, int end, F func, const bool& async = true)
{
	if (async)
	{
		std::vector<int> x;
		if (begin > end)
		{
			for (int i = begin; i > end; i--)
			{
				x.push_back(i);
			}
		}
		else
		{
			for (int i = begin; i < end; i++)
			{
				x.push_back(i);
			}
		}

		std::for_each(std::execution::par, x.begin(), x.end(), func);
	}
	else
	{
		if (begin > end)
		{
			for (int i = begin; i > end; i--)
			{
				func(i);
			}
		}
		else
		{
			for (int i = begin; i < end; i++)
			{
				func(i);
			}
		}
	}
}