#pragma once

#include "Vector.hpp"

namespace kmaths
{
	template<typename T>
	using Vector4 = Vector<T, 4>;

	using Vector4s = Vector4 <   int    >; // signed integer
	using Vector4f = Vector4 <  float   >; // floating point
	using Vector4d = Vector4 <  double  >; // double floating point
	using Vector4u = Vector4 < unsigned >; // unsigned integer
}

