#pragma once 

#include "../../../Precompile.h"
#include "../Vectors/Vector4.h"

#include <array>

namespace kMaths
{
	class Matrix4x4
	{
		Matrix4x4()
		{}

		~Matrix4x4()
			= default;

		void Identity()
		{
			for (auto& mat : matrix)
				mat.Zero();
			
			matrix[0][0] = 1;
			matrix[1][1] = 1;
			matrix[2][2] = 1;
			matrix[3][3] = 1;
		}

	private:
		std::array<Vector4f, 4> matrix;
	};
}
