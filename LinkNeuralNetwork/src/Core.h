#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <execution>

#define LNN_BUILD_DLL 0

#if LNN_BUILD_DLL
#define LNN_API __declspec(dllexport)
#else
#define LNN_API
#endif

typedef unsigned int uint;

namespace nn {

	inline std::vector<std::string> FileToArray(std::fstream& file)
	{
		std::vector<std::string> out;

		std::string str;
		while (file >> str)
		{
			out.push_back(str);
		}

		return out;
	}

	inline double map(const double& value, const double& inputMin, const double& inputMax, const double& outputMin, const double& outputMax)
	{
		return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (value - inputMin);
	}

	inline double randRange(const double& min, const double& max)
	{
		return min + (rand() / (RAND_MAX / ((max + 0) - min)));
	}

	// [begin, end)
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


}