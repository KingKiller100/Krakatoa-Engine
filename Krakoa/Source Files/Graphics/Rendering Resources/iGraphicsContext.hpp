#pragma once

#include "../../EngineMacros.hpp"

namespace krakoa::graphics
{
	class iContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
