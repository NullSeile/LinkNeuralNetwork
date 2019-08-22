#pragma once

#include "Core.h"
#include "Vec2.h"

namespace nn {

	class LNN_API Link
	{
	public:
		Link();
		Link(const Vec2<double>& initialRange);

		double weight;
	};

}