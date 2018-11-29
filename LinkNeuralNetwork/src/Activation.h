#pragma once

#include "Core.h"

namespace nn {

	struct LNN_API Activation
	{
		Activation(const std::string& id, const std::function<double(const double& x)>& funct, const std::function<double(const double& x)>& derivate);

		// Function
		const std::function<double(const double& x)> funct;
		// Derivative
		const std::function<double(const double& x)> derivate;

		// Name
		const std::string id;

		// Presets
		static const Activation sigmoid;
		static const Activation hyperbolicTangent;
		static const Activation none;
	};
}