#include "Activation.h"

namespace nn {

	const Activation Activation::sigmoid
	(
		"sigmoid",
		[](const double& x)->double { return 1.f / (1.f + exp(-x)); },
		[](const double& x)->double { return (1.f / (1.f + exp(-x))) * (1 - (1.f / (1.f + exp(-x)))); }
	);

	const Activation Activation::hyperbolicTangent
	(
		"tanh",
		[](const double& x)->double { return tanh(x); },
		[](const double& x)->double { return 1 - (tanh(x) * tanh(x)); }
	);

	const Activation Activation::none
	(
		"none",
		[](const double& x)->double { return x; },
		[](const double& x)->double { return 1; }
	);

	Activation::Activation(const std::string& id, const std::function<double(const double& x)>& funct, const std::function<double(const double& x)>& derivate)
		: funct(funct)
		, derivate(derivate)
		, id(id)
	{

	}

}