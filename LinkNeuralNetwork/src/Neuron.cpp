#include "Neuron.h"

namespace nn {

	Neuron::Neuron(const Activation& activation)
		: value(1)
		, raw(0)
		, error(0)
		, activation(activation)
	{

	}

}