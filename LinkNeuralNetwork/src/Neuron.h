#pragma once

#include "Core.h"
#include "Activation.h"

namespace nn {

	class LNN_API Neuron
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