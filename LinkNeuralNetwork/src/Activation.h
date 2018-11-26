#pragma once

#include "Core.h"

namespace nn {

	struct NNN_API Activation
	{
		Activation(const std::string& id, const std::function<double(const double& x)>& funct, const std::function<double(const double& x)>& derivate);

		const std::function<double(const double& x)> funct;
		const std::function<double(const double& x)> derivate;

		const std::string id;

		static const Activation sigmoid;
		static const Activation hyperbolicTangent;
		static const Activation none;
	};
}