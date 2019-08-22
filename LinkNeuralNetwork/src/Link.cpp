#include "Link.h"

namespace nn {

	Link::Link(const Vec2<double>& initialRange)
		: weight(randRange(initialRange.min, initialRange.max))
	{
	}

}