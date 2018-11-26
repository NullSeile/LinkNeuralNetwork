#pragma once

#include "Core.h"
#include "Activation.h"

namespace nn {

	class NNN_API Neuron
	{
	private:


	public:
		Neuron(const Activation& activation);

		double value;
		double raw;
		double error;

		const Activation activation;
	};

}