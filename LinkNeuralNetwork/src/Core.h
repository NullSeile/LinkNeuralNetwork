#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <functional>

#define NNN_BUILD_DLL 0

#if NNN_BUILD_DLL
#define NNN_API __declspec(dllexport)
#else
#define NNN_API
#endif

typedef unsigned int uint;

namespace nn {

	inline double map(const double& value, const double& inputMin, const double& inputMax, const double& outputMin, const double& outputMax)
	{
		return outputMin + ((outputMax - outputMin) / (inputMax - inputMin)) * (value - inputMin);
	}

	inline double randRange(const double& min, const double& max)
	{
		return min + (rand() / (RAND_MAX / ((max + 0) - min)));
	}

}